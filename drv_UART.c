/***************************************************************************//**
  @file     drv_UART.c
  @brief    UART Driver
  @author   Tristan Gonzalez Branca
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h>
#include "drv_UART.h"
#include "isr.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define INITIAL_TIME 800
#define UART_LLIMIT 100
#define UART_ULIMIT 2000
#define SPEED 100

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
UART_RX_Buffer uartRXBuffer = { .head = 0, .tail = 0, .count = 0, .receiving = 0, .rx_flag = 0 };
UART_TX_Buffer uartTXBuffer = {NULL, 0, 0};  // Buffer de transmisi�n inicializado

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

unsigned char *periodicTX;
unsigned char periodicLength;
uint16_t uart_period;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void UART_init(uint8_t periodic_flag){

    /* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
    P1SEL |= BIT1 | BIT2 ;
    P1SEL2 |= BIT1 | BIT2;
    P1DIR |= 1<<2;
    P1REN |= 1<<1; /* Place UCA0 in Reset to be configured */
    P1OUT |= (1<<1) + (1<<2);
    UCA0CTL1 = UCSWRST;

    /* Configure */
    UCA0CTL0 = 0;
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 104; // 1MHz 9600
    UCA0BR1 = 0; // 1MHz 9600 3*256 + 65 = 833
    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 2

    /* Take UCA0 out of reset */
    UCA0CTL1 &= ~UCSWRST;

    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt

    if(periodic_flag){
        setUARTPeriod(INITIAL_TIME);
    }
    //__bis_SR_register(GIE); // interrupts enabled
}


void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength){
    // Guarda el puntero al array y el largo en el buffer
    if(!uartTXBuffer.transmiting){
    uartTXBuffer.str = TxArray;
    uartTXBuffer.length = ArrayLength;
    uartTXBuffer.transmiting = 1;

    UC0IE |= UCA0TXIE;  // Enable USCI_A0 TX interrupt
    }
}

UART_RX_Buffer* UART_getBuffer(void) {
    return &uartRXBuffer;
}

uint8_t UART_connection(void) {
    return uartRXBuffer.rx_flag;
}

void setUARTPeriod(uint16_t period){
    // Setea el periodo de trabajo de UART por interrupcion
    uart_period = period;
    send_to_isr(UARTPeriodic,2*uart_period);
}

void UARTPeriodic(){
 // send message periodically
    while(periodicLength--){ // Loop until StringLength == 0 and post decrement
     while(!(IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready for new data
     UCA0TXBUF = *periodicTX; //Write the character at the location specified py the pointer
     periodicTX++; //Increment the TxString pointer to point to the next character
     }
    UARTSendArray("\n\r",2);
}

void setTXMessage(unsigned char *Text, unsigned char Largo){
    // Setea el mensaje a enviar por UART
    periodicTX = Text;
    periodicLength = Largo;

}

void incrementUARTPeriod(){
    uart_period += SPEED;
    if (uart_period > UART_ULIMIT)
        uart_period = UART_ULIMIT;

    send_to_isr(UARTPeriodic,2*uart_period);
}

void decrementUARTPeriod(){
    uart_period -= SPEED;
    if (uart_period < UART_LLIMIT)
        uart_period = UART_LLIMIT;

    send_to_isr(UARTPeriodic,2*uart_period);
}

void UART_parseData(UART_RX_Buffer* buffer, uint8_t* setpoint, uint8_t* histeresis, uint16_t* data3) {
    uint8_t temp[2] = {0, 0};  // Temporal para almacenar los valores de setpoint y histeresis
    uint16_t temp3 = 0;  // Temporal para almacenar el tercer valor (de hasta 4 d�gitos)
    uint8_t comma_count = 0;
    uint8_t byte;
    uint8_t parsing = 0;

    // Procesa el buffer circular desde `tail` hasta `head`
    while ((buffer->tail != buffer->head) && (buffer->receiving == 0)) {
        byte = buffer->data[buffer->tail];  // Lee el byte actual
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;  // Avanza el �ndice `tail` de manera circular

        if (byte == START_BYTE) {
            // Reinicia el proceso si se detecta un nuevo START_BYTE
            comma_count = 0;
            temp[0] = temp[1] = 0;
            temp3 = 0;
            parsing = 1;  // Inicia el parseo
        } else if (parsing && byte == STOP_BYTE) {
            // Si se detecta STOP_BYTE, finaliza el parseo y asigna los valores
            *setpoint = temp[0];
            *histeresis = temp[1];
            *data3 = temp3;
            parsing = 0;  // Finaliza el parseo
            if(buffer->tail == buffer->head){
                buffer->rx_flag = 0;  // Indica que se ha procesado el paquete completo
            }
            break;
        } else if (parsing) {
            // Procesa el contenido entre START_BYTE y STOP_BYTE
            if (byte == ',') {
                comma_count++;
            } else {
                // Llena el valor correspondiente seg�n el n�mero de comas encontradas
                switch (comma_count) {
                    case 0:
                        temp[0] = temp[0] * 10 + (byte - '0');
                        break;
                    case 1:
                        temp[1] = temp[1] * 10 + (byte - '0');
                        break;
                    case 2:
                        temp3 = temp3 * 10 + (byte - '0');
                        break;
                }
            }
        }
    }
}


void UART_parseTXData(unsigned char *str, float TEMP, uint8_t calefactor){
    uint8_t temp_int = ((uint8_t) TEMP);
    str[0] = temp_int / 10 + '0'; // primer digito
    str[1] = temp_int % 10 + '0'; // segundo digito
    str[2] = '.'; // punto decimal
    str[3] = (int) (TEMP * 10) % 10 + '0';  // primer decimal
    str[4] = (int) (TEMP * 100) % 10 + '0'; // segundo decimal
    str[5] = ','; // coma
    str[6] = calefactor + '0'; // estado del calefactor calefactor
    str[7] = '\n';
}



uint8_t UARTReadByte(void) {
    uint8_t data = 0;

    if (uartRXBuffer.count > 0) {
        data = uartRXBuffer.data[uartRXBuffer.tail];
        uartRXBuffer.tail = (uartRXBuffer.tail + 1) % BUFFER_SIZE;  // Incrementa el �ndice de cola de manera circular
        uartRXBuffer.count--;  // Decrementa el contador de bytes disponibles
    }

    return data;
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        INTERRUPTS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Interrupt BUFFER RX
 */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    uint8_t rxdata;
    if (IFG2 & UCA0RXIFG) { // Chequea si hay datos recibidos
        rxdata = UCA0RXBUF; // Lee el byte recibido

        if (!uartRXBuffer.receiving && rxdata == START_BYTE) {
            uartRXBuffer.receiving = 1; // Comienza la recepci�n si el byte inicial es el de inicio
        }

        if (uartRXBuffer.receiving) {
            // Guarda el byte recibido en el buffer circular
            uartRXBuffer.data[uartRXBuffer.head] = rxdata;
            uartRXBuffer.head = (uartRXBuffer.head + 1) % BUFFER_SIZE;  // Incrementa el �ndice de cabeza de manera circular
            uartRXBuffer.count++;

            if (rxdata == STOP_BYTE) {  // Si se recibe el byte de fin de paquete
                uartRXBuffer.receiving = 0; // Finaliza la recepci�n
                uartRXBuffer.rx_flag = 1;  // Indica que se ha recibido un paquete completo
            }

            if (uartRXBuffer.count >= BUFFER_SIZE) {  // Manejo de overflow
                // Aqu� podr�as implementar l�gica de manejo de overflow si es necesario
                // Por ejemplo, podr�as descartar el dato m�s antiguo, ignorar el nuevo, etc.
                // En este caso, simplemente sobrescribimos el dato m�s antiguo.
                uartRXBuffer.tail = (uartRXBuffer.tail + 1) % BUFFER_SIZE;  // Descartamos el byte m�s antiguo
                uartRXBuffer.count--;  // Mantenemos el conteo de bytes correcto
            }
        }
    }
}


/**
 * @brief Interrupt BUFFER TX
 */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    if (uartTXBuffer.length > 0) {
            // Env�a el siguiente byte desde el buffer
            UCA0TXBUF = *uartTXBuffer.str;

            // Avanza el puntero y decrementa la longitud
            uartTXBuffer.str++;
            uartTXBuffer.length--;
        } else {
            uartTXBuffer.transmiting = 0;
            UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
        }
}

/******************************************************************************/
