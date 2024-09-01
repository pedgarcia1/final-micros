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
 * @brief: Reads a byte of data from EEPROM
 * @param address: Address to read data
 * @return: Data read
 */
uint8_t EEPROM_readData(uint16_t address, uint8_t length);


/*******************************************************************************
 ******************************************************************************/

#endif // EEPROM_H

