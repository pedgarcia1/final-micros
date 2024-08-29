#include "system.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"
#include "isr.h"
#include "eeprom.h"

void AppInit(void);
void AppRun(void);



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
    EEPROM_Init();
}

void AppRun(void)
{

    uint8_t dato = 0x55;

    EEPROM_writeData(100, &dato, 1);

    uint8_t dato_leído;

    EEPROM_readData(100, &dato_leído, 1);

    __delay_cycles(1000000);

}