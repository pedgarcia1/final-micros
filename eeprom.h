/*******************************************************************************
  @file     eeprom.h
  @brief    EEPROM driver's header file
            - 24LC256 EEPROM
            - 32KB of memory
            - 64-byte pages
            - 32-byte write cycle time
            - 400kHz I2C communication
            - PIN 6 -> SCL (UCB0SCL)
            - PIN 5 -> SDA (UCB0SDA)
            - PINS 1, 2, 3 to GND
            - Connects both pins with a pull-up resistor to Vcc (not larger than MSP430 Vcc)
  @author   Ignacio Quintana
 ******************************************************************************/

#ifndef EEPROM_H
#define EEPROM_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h>
#include <stdint.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SETPOINT_LSB_ADDRESS    0x0000
#define SETPOINT_MSB_ADDRESS    0x0001
#define SETPOINT_CS_ADDRESS     0x0002

#define SAMPLETIME_LSB_ADDRESS  0x0003
#define SAMPLETIME_MSB_ADDRESS  0x0004
#define SAMPLETIME_CS_ADDRESS   0x0005

#define HYSTERESIS_LSB_ADDRESS  0x0006
#define HYSTERESIS_MSB_ADDRESS  0x0007
#define HYSTERESIS_CS_ADDRESS   0x0008

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
 * @brief: Initializes EEPROM
 */
void EEPROM_init(void);

/**
 * @brief: Writes a byte of data to EEPROM
 * @param address: Address to write data
 * @param data: Data to be written
 */
void EEPROM_writeData(uint16_t address, uint8_t* data, uint8_t length);

/**
 * @brief Reads multiple bytes of data from the EEPROM.
 * 
 * This function reads a specified number of bytes from the EEPROM at the given 
 * starting address and stores the data in the provided buffer.
 * 
 * @param address The EEPROM memory address from which data should be read.
 *                The address is 16 bits wide, allowing access to a large EEPROM.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read from the EEPROM.
 */
void EEPROM_readData(uint16_t address, uint8_t* data, uint8_t length);


/*******************************************************************************
 ******************************************************************************/

#endif // EEPROM_H

