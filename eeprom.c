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
#include "isr.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define EEPROM_SLAVE_ADDR 0x50 // Connect A2, A1, A0 to GND
#define EEPROM_WRITE_TIME 2000 // 1s write time, una interrupción de timer cada 0.5ms

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
 * @brief: sets the EEPROM writing flag in 0. Removes the function from the isr
 */
void EEPROM_finishWrite(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t EEPROM_writingFlag = FALSE;
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

    uint8_t writeData[MAX_WRITE_LENGTH + 2];
    writeData[0] = addrHigh;
    writeData[1] = addrLow;
    uint8_t i;
    for (i = 0; i < length; i++) {
        writeData[i + 2] = data[i];
    }


    I2C_writeData(writeData, length + 2);

    send_to_isr(EEPROM_finishWrite, EEPROM_WRITE_TIME); // 1s write time
    EEPROM_writingFlag = TRUE;

}

// EEPROM read data function
void EEPROM_readData(uint16_t address, uint8_t* readData, uint8_t length) {
    while (EEPROM_writingFlag); // Wait until the EEPROM is not writing

    uint8_t addrHigh = (address >> 8) & 0xFF;
    uint8_t addrLow = address & 0xFF;

    uint8_t addressData[2] = {addrHigh, addrLow};


    I2C_writeData(addressData, 2);

    I2C_readData(readData, length);

}

uint8_t EEPROM_getWritingFlag(void) {
    return EEPROM_writingFlag;
}

void EEPROM_parseData(enum EEPROM_PARSE parse, uint8_t *data, uint8_t length, uint8_t *setpoint, uint8_t *histeresis, uint16_t *intMuestreo)
{
    uint8_t checksum = 0;
    uint8_t i;

    switch (parse)
    {
    case READ:
        for (i = 0; i < length - 1; i++)
        {
            checksum ^= data[i]; // XOR de todos los datos le�dos
        }

        if (checksum == data[length - 1])
        {
            *setpoint = data[0];
            *histeresis = data[1];
            // data[2] MSB de intMuestreo
            // data[3] LSB de intMuestreo
            *intMuestreo = (data[2] << 8) | data[3];
            temp_setTMuestreo(intMuestreo);
        }
        else
        {
            // Error de checksum ??
        }
        break;
    case WRITE:
        data[0] = *setpoint;
        data[1] = *histeresis;
        data[2] = (*intMuestreo >> 8) & 0xFF;
        data[3] = *intMuestreo & 0xFF;
        data[4] = 0x00;
        // Calcular el checksum sobre los datos (del byte 0 al 4)
        uint8_t i;
        for (i = 0; i < (length - 1); i++)
        {
            checksum ^= data[i]; // XOR de los datos
        }
        data[length - 1] = checksum; // Checksum en pos. 5
        break;
    default:
        break;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void EEPROM_finishWrite(void) {
    EEPROM_writingFlag = FALSE;
    remove_from_isr(EEPROM_finishWrite);
}
