// --------------- INCLUDES --------------- //
#include "DS18B20.h"
#include "GPIO_OW.h"
#include <stdint.h>
#include "isr.h"

// --------------- DEFINES --------------- //
#define CONV  ((float)   0.25)
#define TRUNC ((uint8_t) 0xFE)
#define DELAY_US(DELAY)  __delay_cycles(8*DELAY);

// --------------- PROTOTYPES --------------- //
void temp_writeByte(uint8_t byte);
uint8_t temp_readByte(void);
uint8_t temp_CRC8(uint8_t *addr, uint8_t len);
void temp_ISR(void);

// --------------- VARIABLES --------------- //
enum DS1820_STATE t_state = STANDBY;    // STANDBY by default
uint16_t t_muestreo = 1000*2;  // 1 interrupcion de timer cada 0.5ms => una medición cada 1s

// --------------- FUNCTIONS --------------- //
void temp_Init(void){
    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, LOW);

    
}


uint8_t temp_Reset(void){

    // Agregar check del estado del sensor, solo ejectuar si esta en standby
    uint8_t r;

    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, LOW);
    DELAY_US(520);
    PIN_INPUT(ONE_WIRE);
    DELAY_US(70);
    r = !DIGITAL_READ(ONE_WIRE);
    DELAY_US(480-70);
    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, HIGH);
    return r;

    /*
    if(!DIGITAL_READ(ONE_WIRE)){
        DELAY_US(410);
        PIN_OUTPUT(ONE_WIRE);
        DIGITAL_WRITE(ONE_WIRE, HIGH);
        return 1;
    }
    else{
        PIN_OUTPUT(ONE_WIRE);
        DIGITAL_WRITE(ONE_WIRE, HIGH);
        return 0;
    }
    */

    t_state = STANDBY;
}

void temp_writeByte(uint8_t byte){

    uint8_t bit;
    uint8_t i;

    for(i=0; i<8; i++){
        bit = ((byte>>i)&1);

        if (bit) {
            DIGITAL_WRITE(ONE_WIRE, LOW);
            DELAY_US(10);
            DIGITAL_WRITE(ONE_WIRE, HIGH);  // Writes HIGH if BIT is 1
            DELAY_US(55);
        }
        else {
            DIGITAL_WRITE(ONE_WIRE, LOW);
            DELAY_US(65);
            DIGITAL_WRITE(ONE_WIRE, HIGH);  // Writes LOW if BIT is 0
            DELAY_US(5);
        }
    }
}

uint8_t temp_readByte(void){

    uint8_t  i;
    uint8_t  byte = 0;
    uint8_t  bit;

    for(i=0; i<8; i++){

        PIN_OUTPUT(ONE_WIRE);
        DIGITAL_WRITE(ONE_WIRE, LOW);
        DELAY_US(3);                    // MASTER sets OUTPUT to LOW to initiate a READ slot
        PIN_INPUT(ONE_WIRE);
        DELAY_US(10);                   // MASTER waits T<15us to read INPUT signal
        bit = DIGITAL_READ(ONE_WIRE);
        DELAY_US(53);                   // MASTER waits T>60us (66) to end READ slot
        byte |= (bit<<(i));
       }

    return byte;
}

// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 0.
void temp_StartConversion(uint8_t power){
    temp_writeByte(SKIPROM);
    temp_writeByte(CONVERTTEMP);
    if(!power){
        PIN_OUTPUT(ONE_WIRE);
        DIGITAL_WRITE(ONE_WIRE, HIGH);
    }

    // start timer

    t_state = CONVERTING_T;
    send_to_timer_isr(temp_isr, t_muestreo);
}

// Read the scratchpad data from the DS18B20
// Input: data - pointer to 9 bytes array
float temp_ReadTemperature(void){

    uint8_t data[9];
    float TEMP, CountRemain, CountPerC;

    temp_writeByte(SKIPROM);
    temp_writeByte(READSCRATCHPAD);

    uint8_t i = 0;
    for(i=0; i<9; i++){
        data[i] = temp_readByte();
    }

    uint8_t crc;
    crc = temp_CRC8(data,8);
    if(crc == data[8]) // Checks CRC against byte 9 of read data, calculate TEMP only if data is correct
    {
        CountRemain =  (float)   data[6];
        CountPerC   =  (float)   data[7];
        TEMP        =  ((float) (data[0] & TRUNC))/2 + ((CountPerC - CountRemain)/CountPerC) - CONV;

        t_state = STANDBY; // Retry temperature read
    }else{
        TEMP = -1.0;
        t_state = READING_ERROR; // Retry temperature read
    }

    return TEMP;

}

// Check if the sensor is powered by parasite power or external power supply
// Output: 0 parasite power, 1 external power supply (Vdd)
//  WIP, not working testing pending
uint8_t temp_CheckPower(void){

    uint8_t power;

    temp_writeByte(SKIPROM);
    temp_writeByte(READ_POWER_SUPPLY);

    // DELAY_US(3);

    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, LOW);
    DELAY_US(3); // MASTER sets OUTPUT to LOW to initiate a READ slot
    PIN_INPUT(ONE_WIRE);
    DELAY_US(10); // MASTER waits T<15us to read INPUT signal
    power = DIGITAL_READ(ONE_WIRE); // 0 parasite power, 1 external power supply (Vdd)
    DELAY_US(53); // MASTER waits T>60us (66) to end READ slot

    return power;
}

// Get the state of the sensor
// Output: STANDBY, CONVERTING_T, CONVERSION_DONE
uint8_t temp_CheckState(void){
    return t_state;
}

// Set the state of the sensor
// Input: state - STANDY, CONVERTING_T, CONVERSION_DONE
void temp_SetState(enum DS1820_STATE state){
    t_state = state;
}

// Calculates 8 bit CRC, to check if data is valid
// Input: pointer to data, usually len = 8
uint8_t temp_CRC8(uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;
    uint8_t i;
	while (len--) {
		uint8_t inbyte = *addr++;
		for (i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}


void temp_isr(void){
    temp_SetState(CONVERSION_DONE);
    remove_from_timer_isr(temp_isr);
}


// void temp_ISR(void){
//     // 1 interrupcion de timer cada 0.5ms
//         if (temp_CheckState() == CONVERTING_T)
//         {
//             count++;
//             if (count >= 1000 * 2) // 1 interrupcion de timer cada 0.5ms
//             {
//                 temp_SetState(CONVERSION_DONE);
//                 count = 0;
//             }
//         }
// }
