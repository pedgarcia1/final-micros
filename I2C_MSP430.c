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
#include <stdint.h>
#include <stdbool.h>

#include <msp430.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define I2C_SCL_PIN BIT6
#define I2C_SDA_PIN BIT7
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
 * @brief: Sends a start condition
 * @param action: True if master is transmitting, False if master is receiving
*/
void I2C_start(bool action);

/**
 * @brief: Sends a stop condition
*/
void I2C_stop();

/**
 * @brief: Sends a byte of data
 * @param data: Data to be sent
*/
void I2C_writeByte(uint8_t data);

/**
 * @brief: Reads a byte of data
 * @return: Data read
*/
uint8_t I2C_readByte(void);

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



/***  COMENTARIOS ÚTILES TEO
El micro tiene un USCI (Universal Serial Communication Interface), osea un módulo pensado para comuicaciones seriales que se activa en diferentes modos. 
En particular, nuestro micro tiene el USCI_B0, que es el que se usa para I2C.
En I2C hay dos cables, uno de clock y otro de datos, que se conectan a los pines 1.6 y 1.7 porque así lo dice el datasheet.
El módulo tiene mil opciones (master, slave, i2c, spi, tipo de clock, etc) que se configuran con los registros UCB0CTL0 y UCB0CTL1.

// SFR_8BIT(UCB0CTL0);                           /* USCI B0 Control Register 0 */
// SFR_8BIT(UCB0CTL1);                           /* USCI B0 Control Register 1 */
// SFR_8BIT(UCB0BR0);                            /* USCI B0 Baud Rate 0 */
// SFR_8BIT(UCB0BR1);                            /* USCI B0 Baud Rate 1 */
// SFR_8BIT(UCB0I2CIE);                          /* USCI B0 I2C Interrupt Enable Register */
// SFR_8BIT(UCB0STAT);                           /* USCI B0 Status Register */
// SFR_8BIT(UCB0RXBUF);                          /* USCI B0 Receive Buffer */
// SFR_8BIT(UCB0TXBUF);                          /* USCI B0 Transmit Buffer */
// SFR_16BIT(UCB0I2COA);                         /* USCI B0 I2C Own Address */
// SFR_16BIT(UCB0I2CSA);                         /* USCI B0 I2C Slave Address */
/** 
Configuring and reconfiguring the USCI module should be done when UCSWRST is set to avoid
unpredictable behavior. Setting UCSWRST in I C mode has the following effects:
• I C communication stops
• SDA and SCL are high impedance
• UCBxI2CSTAT, bits 6-0 are cleared
• UCBxTXIE and UCBxRXIE are cleared
• UCBxTXIFG and UCBxRXIFG are cleared
• All other bits and registers remain unchanged
*/

// I2C initialization function
void I2C_init() {
    // Configure I2C pins
    P1SEL   |= I2C_SCL_PIN + I2C_SDA_PIN;     // Select I2C function for these pins
    P1SEL2  |= I2C_SCL_PIN + I2C_SDA_PIN;    
    //P1REN   |= I2C_SCL_PIN + I2C_SDA_PIN;     // Enable pull-up resistor. Not sure if done by code or hardware (creo que se hace por hardware)

    // Configure I2C module
    UCB0CTL1 |= UCSWRST;                    // Enable software reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC + ~UCSLA10;   // Master mode, I2C mode, synchronous mode, 7-bit slave address
    UCB0CTL1 = UCSSEL_2 + UCSWRST;          // SMCLK, keep in reset state
    UCB0BR0 = 80;                           // Set clock divider for desired SCL frequency (100 kHz)
    UCB0BR1 = 0;   
    // The 16-bit value of (UCBxBR0 + UCBxBR1 × 256) forms the prescaler value. (ahora esta puesto en 80)

    //UCB0I2CSA = slaveAddr;                  // Set slave address
    UCB0CTL1 &= ~UCSWRST;                   // Release software reset
    IE2 |= UCB0TXIE + UCB0RXIE;             // Enable transmit and receive interrupts

}
void I2C_switchSlave(uint8_t slaveAddr) {
    UCB0I2CSA = slaveAddr;                  // Set slave address
}

// I2C write data function
uint8_t I2C_writeData(uint8_t* data, uint8_t length) {
    I2C_start(TRANSMIT);                        // Generate start condition
    
    uint16_t retryCount = 0;

    for (uint8_t i = 0; i < length; i++) {
        I2C_writeByte(data[i]);    // Generate stop condition

                     
        if ((UCB0STAT & UCNACKIFG) && retryCount < 0xFFFF ) {  // Check for NACK
            UCB0STAT &= ~UCNACKIFG;  // Clear NACK flag
            I2C_start(TRANSMIT);                // Generate start condition again
            i--;  // Repeat current byte
            retryCount++;  // Increment retry counter

        } else if (retryCount >= 0xFFFF) {
            return 1;  // Return error code                    
        }
    }
    I2C_stop(TRANSMIT); // Generate stop condition
    return 0; // Return success code
}

// I2C read data function
uint8_t I2C_readData(uint8_t slaveAddr, uint8_t* data, uint8_t length) {
    I2C_start(RECEIVE);                     // Generate start condition
    
    if (UCB0STAT & UCNACKIFG) {             // Check for NACK
        UCB0STAT &= ~UCNACKIFG;             // Clear NACK flag
        uint16_t retryCounter = 0;
        while (retryCounter < 0xFFFF) {
            I2C_start(RECEIVE);             // Generate start condition again
            if (!(UCB0STAT & UCNACKIFG)) {   // Check for NACK
                break;
            }
            retryCounter++;                  // Increment retry counter
            UCB0STAT &= ~UCNACKIFG;             // Clear NACK flag
        }
        if (retryCounter >= 0xFFFF) {
            return 1; // Return error code
        }
        

    }
    I2C_start(RECEIVE);                            // Generate repeated start condition
    I2C_writeByte((slaveAddr << 1) | 0x01); // Send slave address with read bit
    for (uint8_t i = 0; i < length - 1; i++) {
        data[i] = I2C_readByte();           // Read data byte
    }
    data[length - 1] = I2C_readByte();      // Read last data byte
    I2C_stop(RECEIVE);                      // Generate stop condition
    return 0; // Return success code
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// I2C start condition function
void I2C_start(bool action) {
    if (action) {
        UCB0CTL1 |= UCTR;           // Set as transmitter
    } else {
        UCB0CTL1 &= ~UCTR;          // Set as receiver
    }
    UCB0CTL1 |= UCTXSTT;            // Generate start condition
    while (UCB0CTL1 & UCTXSTT);     // Wait for start condition to be transmitted. This bit is automatically cleared by hardware
}

// I2C stop condition function
void I2C_stop(bool action) {
    //NACK if RECEIVING
    if (!action) {
        UCB0CTL1 |= UCTXNACK;       // Generate NACK condition
    }
    
    UCB0CTL1 |= UCTXSTP;            // Generate stop condition
    while (UCB0CTL1 & UCTXSTP);     // Wait for stop condition to be transmitted. This bit is automatically cleared by hardware
}

// I2C write byte function
void I2C_writeByte(uint8_t data) {
    UCB0TXBUF = data;               // Load data into transmit buffer
    while (!(IFG2 & UCB0TXIFG));    // Wait for transmit buffer to be empty
}

// I2C read byte function
uint8_t I2C_readByte() {
    while (!(IFG2 & UCB0RXIFG));    // Wait for receive buffer to be full
    return UCB0RXBUF;               // Return received data
}
