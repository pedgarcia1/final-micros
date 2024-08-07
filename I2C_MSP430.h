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
void i2c_init();

/**
 * @brief: Sends multiple bytes of data
 * @param data: Pointer to data to be sent
 * @param length: Number of bytes to be sent
*/
void i2c_writeData(uint8_t* data, uint8_t length);

/**
 * @brief: Reads multiple bytes of data
 * @param data: Pointer to data to be read
 * @param length: Number of bytes to be read
*/
void i2c_readData(uint8_t* data, uint8_t length);

/**
 * @brief: Switches the I2C slave address
 * @param slaveAddr: New slave address
*/
void i2c_switchSlave(uint8_t slaveAddr);
/*******************************************************************************
 ******************************************************************************/

#endif // I2C_MSP430_H

