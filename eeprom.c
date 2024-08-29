/*******************************************************************************
  @file     eeprom.c
  @brief    EEPROM driver's source file
            - 24LC256 EEPROM
            - 256KB of memory
            - 64-byte pages
            - 32-byte write cycle time
            - 400kHz I2C communication
            - PIN 6 -> SCL (UCB0SCL)
            - PIN 5 -> SDA (UCB0SDA)
            - PINS 1, 2, 3 to GND
            - Connects both pins with a pull-up resistor (10k - 100 kHz / 2k - 400 kHz & 1 MHz) to Vcc (not larger than MSP430 Vcc)
  @author   Ignacio Quintana
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "I2C_MSP430.h"
#include "eeprom.h"
#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define EEPROM_SLAVE_ADDR 0x50 // Connect A2, A1, A0 to GND

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

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

void EEPROM_init() {
    I2C_init();
    I2C_switchSlave(EEPROM_SLAVE_ADDR);
}

// EEPROM write byte function
void EEPROM_writeData(uint16_t address, uint8_t* data, uint8_t length) {
    uint8_t addrHigh = (address >> 8) & 0xFF;
    uint8_t addrLow = address & 0xFF;

    uint8_t writeData[3] = {addrHigh, addrLow, data};
    I2C_writeData(writeData, 3);

    // Wait for write cycle to complete
    __delay_cycles(10000);
}

// EEPROM read byte function
uint8_t EEPROM_readData(uint16_t address, uint8_t* data, uint8_t length) {
    uint8_t addrHigh = (address >> 8) & 0xFF;
    uint8_t addrLow = address & 0xFF;
    uint8_t readData;

    uint8_t addressData[2] = {addrHigh, addrLow};
    I2C_writeData(addressData, 2);  

    I2C_readData(&readData, 1);

    return readData;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
