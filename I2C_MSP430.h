/*******************************************************************************
  @file     I2C_MSP430.h
  @brief    I2C communication driver's header file.
            - PIN 1.6 -> SCL (UCB0SCL)
            - PIN 1.7 -> SDA (UCB0SDA)
            - Connects both pins with a pull-up resistor to Vcc (not larger than MSP430 Vcc)
  @author   Ignacio Quintana
 ******************************************************************************/

#ifndef I2C_MSP430_H
#define I2C_MSP430_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum I2C_error_codes {
    SUCCESS = 0,
    ADDRESS_NACK_CODE,
    TIMEOUT_START_CODE,
    TRANSMIT_NACK_CODE,
    TIMEOUT_TRANSMIT_CODE,
    TIMEOUT_RECEIVE_CODE,
    TIMEOUT_STOP_CODE
};
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief: Initializes I2C communication
*/
void I2C_init();

/**
 * @brief Sends multiple bytes of data over I2C.
 * 
 * This function transmits a specified number of bytes to a slave device
 * using the I2C communication protocol.
 * 
 * @param data Pointer to the buffer holding the data to be transmitted.
 * @param length Number of bytes to be transmitted from the data buffer.
 * 
 * @return 0 if successful, or an appropriate error code if a failure occurs.
 *         Possible error codes include:
 *         - TRANSMIT_NACK_CODE: NACK received after transmitting a byte.
 *         - TIMEOUT_START_CODE: Timeout while waiting for the start condition.
 *         - TIMEOUT_TRANSMIT_CODE: Timeout while waiting for the transmit buffer.
 *         - TIMEOUT_STOP_CODE: Timeout while waiting for the stop condition.
 */
uint8_t I2C_writeData(uint8_t* data, uint8_t length);

/**
 * @brief Reads multiple bytes of data over I2C.
 * 
 * This function receives a specified number of bytes from a slave device
 * using the I2C communication protocol.
 * 
 * @param data Pointer to the buffer where the received data will be stored.
 * @param length Number of bytes to be received and stored in the data buffer.
 * 
 * @return 0 if successful, or an appropriate error code if a failure occurs.
 *         Possible error codes include:
 *         - TIMEOUT_START_CODE: Timeout while waiting for the start condition.
 *         - TIMEOUT_RECEIVE_CODE: Timeout while waiting for the receive buffer.
 *         - TIMEOUT_STOP_CODE: Timeout while waiting for the stop condition.
 */
uint8_t I2C_readData(uint8_t* data, uint8_t length);


/**
 * @brief: Switches the I2C slave address
 * @param slaveAddr: New slave address
*/
void I2C_switchSlave(uint8_t slaveAddr);
/*******************************************************************************
 ******************************************************************************/

#endif // I2C_MSP430_H

