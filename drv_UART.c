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
UART_Buffer uartRXBuffer = {{0}, 0, 0, 0};  // Buffer de recepcion


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
 // Send number of bytes Specified in ArrayLength in the array at using the hardware UART 0
 // Example usage: UARTSendArray("Hello", 5);
 // int data[2]={1023, 235};
 // UARTSendArray(data, 4); // Note because the UART transmits bytes it is necessary to send two bytes for each integer hence the data length is twice the array length

while(ArrayLength--){ // Loop until StringLength == 0 and post decrement
    while(!(IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready for new data
    UCA0TXBUF = *TxArray; //Write the character at the location specified py the pointer
    TxArray++; //Increment the TxString pointer to point to the next character
 }
}

UART_Buffer* UART_getBuffer(void) {
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

void UART_parseData(UART_Buffer* buffer, uint8_t* data1, uint8_t* data2, uint8_t* data3) {
    uint8_t index = 0;
    uint8_t comma_count = 0;
    uint8_t temp[3] = {0, 0, 0};  // Temporal para almacenar los tres valores

    for (index = 0; index < buffer->index; index++) {
        if (buffer->data[index] == ',') {
            comma_count++;
        } else {
            switch (comma_count) {
                case 0:
                    temp[0] = temp[0] * 10 + (buffer->data[index] - '0');
                    break;
                case 1:
                    temp[1] = temp[1] * 10 + (buffer->data[index] - '0');
                    break;
                case 2:
                    temp[2] = temp[2] * 10 + (buffer->data[index] - '0');
                    break;
            }
        }
    }

    // Asignar los valores a las variables de salida
    *data1 = temp[0];
    *data2 = temp[1];
    *data3 = temp[2];
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
    // RXFlag = 1;
    uint8_t rxdata;
    if (IFG2 & UCA0RXIFG) { // Chequea si hay datos recibidos
        rxdata = UCA0RXBUF; // Lee el byte recibido
        if (!uartRXBuffer.receiving && rxdata == START_BYTE) { // Si el primer byte es el de inicio comienza la recepcion
            uartRXBuffer.receiving = 1;
            uartRXBuffer.index = 0;  // Reinicia el índice
        } else if (uartRXBuffer.receiving) {
            uartRXBuffer.data[uartRXBuffer.index++] = rxdata;

            if (rxdata == STOP_BYTE) {
                /* if (uartRXBuffer.index >= 2 && uartRXBuffer.data[uartRXBuffer.index - 2] == STOP_BYTE) {
                    uint8_t sum = checksum(uartRXBuffer.data, uartRXBuffer.index - 3);
                    uartRXBuffer.receiving = 0;
                    if (sum == uartRXBuffer.data[uartRXBuffer.index - 3]) { // Datos recibidos correctamente
                        uartRXBuffer.rx_flag = 1;  // Indica que la recepción ha terminado
                    } else { // Error en los datos recibidos
                        uartRXBuffer.index = 0;
                    }
                } */
               // Por ahora, asumo que los datos llegan correctamente implementar checksum
                uartRXBuffer.receiving = 0;
                uartRXBuffer.rx_flag = 1;
                uartRXBuffer.index--; // IMPORTANTE

            /* } else if (uartRXBuffer.index >= sizeof(uartRXBuffer.data)) {
                // Buffer overflow, resetear
                uartRXBuffer.receiving = 0;
                uartRXBuffer.index = 0;
            }
            */
            // Idem, asumo que no hay overflow, implementar
            }
        }
    }

}

/******************************************************************************/
