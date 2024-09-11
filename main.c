
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
   EEPROM_init();

}

void AppRun(void)
{
    #define LARGO_ENV 3
    #define LARGO_READ 3

    
    uint8_t envio[LARGO_ENV] = {0x10, 0x20, 0x30};
    uint8_t direccion = 0x01;
    uint8_t datos_leidos[LARGO_READ];

    EEPROM_writeData(direccion, envio, LARGO_ENV);

    uint8_t debug;

    EEPROM_readData(direccion, datos_leidos, LARGO_READ);
    uint8_t i;

    for (i = 0; i < LARGO_READ; i++)
    {
        debug = datos_leidos[i];
    }

    __delay_cycles(1000000);
    
}

