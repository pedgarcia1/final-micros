#ifndef _DS18B20_H_
#define _DS18B20_H_

// --------------- INCLUDES --------------- //
#include <msp430.h>
// #include <Board.h>
#include <stdint.h>


// --------------- DEFINES --------------- //
// One Wire pin
#define ONE_WIRE    1,0
// Despues pasar a board o system .h

// Onw Wire commands
#define SKIPROM             0xCC
#define READSCRATCHPAD      0xBE
#define CONVERTTEMP         0x44
#define READ_POWER_SUPPLY   0xB4
#define WRITE_SCRATCHPAD    0x4E

// Device resolution
#define TEMP_9_BIT  0x1F //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit


// Sensor state machine enum
enum DS1820_STATE {
    STANDBY,
    CONVERTING_T,
    CONVERSION_DONE,
    READING_ERROR
};


// --------------- PROTOTYPES --------------- //
void    temp_Init(void);
uint8_t temp_Reset(void);
void    temp_StartConversion(uint8_t power);
float   temp_ReadTemperature(void);
uint8_t temp_CheckPower(void);
void    temp_SetState(enum DS1820_STATE state);
uint8_t temp_CheckState(void);
void temp_SetResolution(uint8_t resolution);

#endif // _DS18B20_H_
