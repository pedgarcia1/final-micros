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

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

volatile unsigned char RXChar;
static uint8_t RXFlag;
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

    /* Use Calibration values for 1MHz Clock DCO*/
    //DCOCTL = 0;
    //BCSCTL1 = CALBC1_1MHZ;
    //DCOCTL = CALDCO_1MHZ;

    /* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
    P1SEL |= BIT1 | BIT2 ;
    P1SEL2 |= BIT1 | BIT2;
    P1DIR |= 1<<2;
    P1REN |= 1<<1; /* Place UCA0 in Reset to be configured */
    P1OUT |= (1<<1) + (1<<2);
    UCA0CTL1 = UCSWRST;

    /* Configure */
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 104; // 8MHz 9600
    UCA0BR1 = 0; // 8MHz 9600 3*256 + 65 = 833
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

uint8_t getRXStatus(){
    // Devuelve el estado del flag de recepcion
    return RXFlag;
}

void resetRXStatus(){
    // Resetea el flag de recepcion a 0 (no hay recepcion)
    RXFlag=0;
}

uint8_t getChar(){
    // Devuelve el caracter recibido
    return RXChar;
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
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
 RXFlag=1;
 RXChar=UCA0RXBUF;
}

/******************************************************************************/

