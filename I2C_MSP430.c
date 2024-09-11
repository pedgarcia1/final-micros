/*******************************************************************************
  @file     I2C_MSP430.c
  @brief    I2C communication driver.
            - PIN 1.6 -> SCL (UCB0SCL)
            - PIN 1.7 -> SDA (UCB0SDA)
            - Connect both pins with a pull-up resistor (10k) to Vcc (not larger than MSP430 Vcc)
  @author   Ignacio Quintana
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "I2C_MSP430.h"
#include "hardware.h"
#include "GPIO_OW.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define TRANSMIT true
#define RECEIVE false
#define WRITE 0
#define READ 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief  Waits for the I2C start condition to be successfully transmitted.
 *         Handles NACK from the slave and timeouts.
 * @return 0 if successful, ADDRESS_NACK_CODE if NACK is received,
 *         TIMEOUT_START_CODE if a timeout occurs.
 */
uint8_t wait_for_start(void);

/**
 * @brief  Waits for the I2C transmit buffer to be ready for the next byte.
 *         Checks for NACK conditions and handles timeouts.
 * @return 0 if successful, TRANSMIT_NACK_CODE if NACK is received,
 *         TIMEOUT_TRANSMIT_CODE if a timeout occurs.
 */
uint8_t wait_for_transmit(void);

/**
 * @brief  Waits for the I2C receive buffer to be ready for the next byte.
 *         Handles timeouts.
 * @return 0 if successful, TIMEOUT_RECEIVE_CODE if a timeout occurs.
 */
uint8_t wait_for_receive(void);

/**
 * @brief  Waits for the I2C stop condition to be transmitted.
 * @return 0 if successful, TIMEOUT_STOP_CODE if a timeout occurs.
 */
uint8_t wait_for_stop(void);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
enum I2C_error_codes status;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


// I2C initialization function
void I2C_init() {
    // Configure I2C pins
    P1SEL   |= I2C_SCL_PIN + I2C_SDA_PIN;     // Select I2C function for these pins
    P1SEL2  |= I2C_SCL_PIN + I2C_SDA_PIN;    
    

    // Configure I2C module
    UCB0CTL1 |= UCSWRST;                    // Enable software reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Master mode, I2C mode, synchronous mode, 7-bit slave address
    UCB0BR0 = 80;                           // Set clock divider for desired SCL frequency (100 kHz)
    UCB0BR1 = 0;   
    // The 16-bit value of (UCBxBR0 + UCBxBR1 × 256) forms the prescaler value. (ahora esta puesto en 80)
    UCB0CTL1 = UCSSEL_2;          // SMCLK, remove reset
    UCB0CTL1 &= ~UCSWRST;                   // Release software reset

    IE2 &= ~(UCB0TXIE | UCB0RXIE);    // Enable I2C interrupt         
    IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);




}

// I2C switch slave function
void I2C_switchSlave(uint8_t slaveAddr) {
    UCB0CTL1 |= UCSWRST;                    // Enable software reset
    UCB0I2CSA = slaveAddr;                  // Set slave address
    UCB0CTL1 &= ~UCSWRST;                   // Release software reset
}


// I2C write data function
uint8_t I2C_writeData(uint8_t* data, uint8_t length) {

    UCB0CTL1 |= UCTR;  // Set as transmitter
    UCB0CTL1 |= UCTXSTT;            // Generate start condition
    UCB0TXBUF = data[0];               // Load data into transmit buffer
    

    if (status = wait_for_start()) return status;
    
    if (status = wait_for_transmit()) return status;
    
    if (length > 1) {
        uint8_t i;
        for ( i = 1; i < length; i++) {
            UCB0TXBUF = data[i];  // Load data into transmit buffer
            if (status = wait_for_transmit()) return status;
        } 
    }
    

    UCB0CTL1 |= UCTXSTP;            // Generate stop condition
    if (status = wait_for_stop()) return status;

    
    return SUCCESS; // Return success code
}

// I2C read data function
uint8_t I2C_readData(uint8_t* data, uint8_t length) {

    UCB0CTL1 &= ~UCTR;                      // Set as receiver
    UCB0CTL1 |= UCTXSTT;                    // Generate start condition
    if (status = wait_for_start()) return status;

    uint8_t i;
    for (i = 0; i < length; i++) {
        if (status = wait_for_receive()) return status;
        data[i] = UCB0RXBUF;           // Read data byte
    }

   // UCB0CTL1 |= UCTXNACK;       // Generate NACK condition
    UCB0CTL1 |= UCTXSTP;        // Generate stop condition
    if (status = wait_for_stop()) return status;

    return SUCCESS; // Return success code
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

uint8_t wait_for_start(void) {
    unsigned long timeout = 2000000;  // Larger timeout value
    while (UCB0CTL1 & UCTXSTT && timeout--) {
        if (UCB0STAT & UCNACKIFG) {
            UCB0CTL1 |= UCTXSTP;            // Generate stop condition
            return ADDRESS_NACK_CODE;  // Return error code if NACK received
        }
    }
    if (timeout == 0) {
        return TIMEOUT_START_CODE;  // Timeout error
    }
    else {
        return SUCCESS;  // Success code
    }
}


uint8_t wait_for_transmit (void) {
    unsigned long timeout = 2000000;  // Larger timeout value
    while (!(IFG2 & UCB0TXIFG) && timeout--) {
        if (UCB0STAT & UCNACKIFG) {
            UCB0CTL1 |= UCTXSTP;            // Generate stop condition
            return TRANSMIT_NACK_CODE;  // Return error code if NACK received
        }
    }
    if (timeout == 0) {
        return TIMEOUT_TRANSMIT_CODE;  // Timeout error
    }
    else {
        return SUCCESS;  // Success code
    }
}


uint8_t wait_for_receive(void) {
    unsigned long timeout = 2000000;  // Larger timeout value
    while (!(IFG2 & UCB0RXIFG) && timeout--);
    if (timeout == 0) {
        return TIMEOUT_RECEIVE_CODE;  // Timeout error
    }
    else {
        return SUCCESS;  // Success code
    }
}


uint8_t wait_for_stop(void) {
    unsigned long timeout = 2000000;  // Larger timeout value
    while (UCB0CTL1 & UCTXSTP && timeout--);
    if (timeout == 0) {
        return TIMEOUT_STOP_CODE;  // Timeout error
    }
    else {
        return SUCCESS;  // Success code
    }
}





