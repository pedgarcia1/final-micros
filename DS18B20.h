#ifndef _DS18B20_H_
#define _DS18B20_H_

// --------------- INCLUDES --------------- //
#include <msp430.h>
#include <stdint.h>
// #include <Board.h>
#include <GPIO_OW.h>

// --------------- DEFINES --------------- //
// One Wire protocol
#define ONE_WIRE    1,0
// Despues pasar a board o system .h

#define SKIPROM             0xCC
#define READSCRATCHPAD      0xBE
#define CONVERTTEMP         0x44

#define DELAY_US(DELAY)  __delay_cycles(8*DELAY);

// --------------- PROTOTYPES --------------- //
void    temp_Init(void);
void    temp_writeByte(uint8_t byte);
uint8_t temp_readByte(void);
uint8_t temp_Reset(void);
void    temp_StartConversion(uint8_t power);
float   temp_ReadTemperature(void);

#endif // _DS18B20_H_
