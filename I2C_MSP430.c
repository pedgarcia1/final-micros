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
#include "i2c_msp430.h"
#include <msp430.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define I2C_SCL_PIN BIT6
#define I2C_SDA_PIN BIT7


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
 * @brief: Sends a start condition
 * @param action: True if master is transmitting, False if master is receiving
*/
void i2c_start(bool action);

/**
 * @brief: Sends a stop condition
*/
void i2c_stop(bool action);

/**
 * @brief: Sends a byte of data
 * @param data: Data to be sent
*/
void i2c_writeByte(uint8_t data);

/**
 * @brief: Reads a byte of data
 * @return: Data read
*/
uint8_t i2c_readByte(void);

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

// I2C initialization function
void i2c_init() {
    // Configure I2C pins
    P1SEL   |= I2C_SCL_PIN + I2C_SDA_PIN;     // Select I2C function for these pins
    P1SEL2  |= I2C_SCL_PIN + I2C_SDA_PIN;    
    //P1REN   |= I2C_SCL_PIN + I2C_SDA_PIN;     // Enable pull-up resistor. Not sure if done by code or hardware

    // Configure I2C module
    UCB0CTL1 |= UCSWRST;                    // Enable software reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Master mode, I2C mode, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;          // SMCLK, keep in reset state
    UCB0BR0 = 80;                           // Set clock divider for desired SCL frequency (100 kHz)
    UCB0BR1 = 0;

    //UCB0I2CSA = slaveAddr;                  // Set slave address
    UCB0CTL1 &= ~UCSWRST;                   // Release software reset
    IE2 |= UCB0TXIE + UCB0RXIE;             // Enable transmit and receive interrupts
}

// I2C write data function
void i2c_writeData(uint8_t* data, uint8_t length) {
    i2c_start(TRANSMIT);                        // Generate start condition
    uint8_t i;
    for (i = 0; i < length; i++) {
        i2c_writeByte(data[i]);                 // Send data byte
    }
    i2c_stop(TRANSMIT);                         // Generate stop condition
}

// I2C read data function
void i2c_readData(uint8_t* data, uint8_t length) {
    i2c_start(RECEIVE);                     // Generate start condition

    uint8_t i;
    for (i = 0; i < length - 1; i++) {
        data[i] = i2c_readByte();           // Read data byte
    }
    data[length - 1] = i2c_readByte();      // Read last data byte
    i2c_stop(RECEIVE);                      // Generate stop condition
}

void i2c_switchSlave(uint8_t slaveAddr) {
    UCB0I2CSA = slaveAddr;                  // Set slave address
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// I2C start condition function
void i2c_start(bool action) {
    if (action) {
        UCB0CTL1 |= UCTR;           // Set as transmitter
    } else {
        UCB0CTL1 &= ~UCTR;          // Set as receiver
    }
    UCB0CTL1 |= UCTXSTT;            // Generate start condition
    while (UCB0CTL1 & UCTXSTT);     // Wait for start condition to be transmitted. This bit is automatically cleared by hardware
}

// I2C stop condition function
void i2c_stop(bool action) {
    //NACK if RECEIVING
    if (!action) {
        UCB0CTL1 |= UCTXNACK;       // Generate NACK condition
    }
    
    UCB0CTL1 |= UCTXSTP;            // Generate stop condition
    while (UCB0CTL1 & UCTXSTP);     // Wait for stop condition to be transmitted. This bit is automatically cleared by hardware
}

// I2C write byte function
void i2c_writeByte(uint8_t data) {
    UCB0TXBUF = data;               // Load data into transmit buffer
    while (!(IFG2 & UCB0TXIFG));    // Wait for transmit buffer to be empty
}

// I2C read byte function
uint8_t i2c_readByte() {
    while (!(IFG2 & UCB0RXIFG));    // Wait for receive buffer to be full
    return UCB0RXBUF;               // Return received data
}
