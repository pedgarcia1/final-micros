#include "system.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"
#include "isr.h"
#include "eeprom.h"
#include "GPIO_OW.h"

void AppInit(void);
void AppRun(void);

// I2C protocol
#define I2C_SCL     1,6
#define I2C_SDA     1,7

int otra = 0;

void main(void)
{
    // NO TOCAR
    systemInitFirst();
    boardInit(); // Empty
    AppInit();
    systemInitLast();

    for (;;)
        AppRun();
    // NO TOCAR
}


void AppInit(void)
{
    // Inicializaciones
   EEPROM_init();

}

void AppRun(void)
{

    
    uint8_t dato = 0x14;
    uint8_t dato_leido = 0x1;

    EEPROM_writeData(10, &dato, 1);

    uint8_t debug;

    dato_leido = EEPROM_readByte(20);
    debug = &dato_leido;


    __delay_cycles(1000000);
    
}

