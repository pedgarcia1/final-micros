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
 * @brief: Clears the I2C bus
*/
void I2C_clearBus(void);



/**
 * @brief: Sends multiple bytes of data
 * @param data: Pointer to data to be sent
 * @param length: Number of bytes to be sent
 * @return: 0 if success, 1 if error
*/
uint8_t I2C_writeData(uint8_t* data, uint8_t length);

/**
 * @brief: Reads multiple bytes of data
 * @param data: Pointer to data to be read
 * @param length: Number of bytes to be read
 * @return: 0 if success, 1 if error
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

