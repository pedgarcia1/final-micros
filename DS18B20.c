// --------------- INCLUDES --------------- //
#include <DS18B20.h>

// --------------- DEFINES --------------- //
#define CONV  ((float)   0.25)
#define TRUNC ((uint8_t) 0xFE)

// --------------- FUNCTIONS --------------- //
void temp_Init(void){
    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, LOW);

    // Timer init
}


uint8_t temp_Reset(void){

    PIN_OUTPUT(ONE_WIRE);
    DIGITAL_WRITE(ONE_WIRE, LOW);
    DELAY_US(480);
    PIN_INPUT(ONE_WIRE);
    DELAY_US(70);

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
// parasite power mode) then set 'power' to 1.
void temp_StartConversion(uint8_t power){
    temp_writeByte(SKIPROM);
    temp_writeByte(CONVERTTEMP);
    if(!power){
        PIN_OUTPUT(ONE_WIRE);
        DIGITAL_WRITE(ONE_WIRE, HIGH);
    }
    DELAY_US(750000);
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

    CountRemain =  (float)   data[6];
    CountPerC   =  (float)   data[7];
    TEMP        =  ((float) (data[0] & TRUNC))/2 + ((CountPerC - CountRemain)/CountPerC) - CONV;

    // Check CRC to see if data is valid, implement later

    return TEMP;

}



