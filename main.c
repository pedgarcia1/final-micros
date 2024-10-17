/***************************************************************************//**
  @file     main.c
  @brief    Archivo principal del Tp Final
  @Author   Ignacion Quintana, Teo Nicoletti, Tristan Gonzalez Branca y Pedro Garcia Delucis
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "system.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"
#include "ledBar.h"
#include "DS18B20.h"
#include "drv_UART.h"
#include "isr.h"
#include "pwm.h"
#include "timer.h"
#include "isrTimer.h"
#include "statusLed.h"
#include "eeprom.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void AppInit(void);
void AppInitLast(void);
void AppRun(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static const uint8_t power = 1; // 0 parasite power, 1 external power supply (Vdd)
uint8_t setpoint, histeresis, temp_int, checksumFlag;
uint16_t intMuestreo;
uint8_t calefactor = 0;

uint8_t envio[LARGO_ENV] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t direccion = 0x75; // Random Addres
uint8_t datos_leidos[LARGO_READ];
uint8_t debug;

#define MAX_TEMP    60.0
#define MIN_TEMP    20.0
#define LED_MAX     8.0

unsigned int timeoutPeriod = 6000;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void main(void)
{
    // NO TOCAR
    systemInitFirst();
    boardInit(); // Empty
    AppInit();
    systemInitLast(); // Start WDT timer and interrupts
    AppInitLast(); // Last init depends on WDT timer & interrupts

    for (;;)
        AppRun();
    // NO TOCAR
}

void AppInit(void)
{
    // Inicializacin (se ejecuta 1 sola vez al comienzo)
    timerInitialization(TIMER_PERIOD);
    timerStart();
    temp_Init(); // Inicializa el sensor de temperatura DS18B20
    ledBarInit(); // Inicializa el display de 8 leds y el shift register 74HC595
    UART_init(NO_PERIODIC);
    PWM_Init();
    statusLed_init();
    EEPROM_init();

}

void AppInitLast(void)
{

        timerTimeoutStart(timeoutPeriod);
        while (!timerGetTimeout())
        {
            if (UART_connection()) // Chequea conexcion con PC por UART
            {
                statusLed_PC(NORMAL);

                // Lectura datos de la PC
                UART_RX_Buffer *rxBufferPointer = UART_getBuffer();
                UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
                temp_setTMuestreo(intMuestreo);

                timerTimeoutStop(); // Salir del loop
            }
        }

        if (!UART_connection())
        {
            statusLed_PC(ERROR);

            // Lectura de la EEPROM
            EEPROM_readData(direccion, datos_leidos, LARGO_READ);
            // Parse EEPROM data
            checksumFlag = EEPROM_parseData(READ, datos_leidos, LARGO_READ, &setpoint, &histeresis, &intMuestreo);
            temp_setTMuestreo(intMuestreo);

            if (checksumFlag)
            {
                statusLed_setState(NORMAL); // LED estado normal
            }else{
                statusLed_setState(ERROR); // LED estado error
            }
        }
}

void AppRun(void)
{
    // Loop (se ejecuta constantemente en un ciclo infinito)
    uint8_t presence = 0;
    float TEMP = 0;

    switch (temp_CheckState())
    {
    case STANDBY:
        presence = temp_Reset();
        if (presence)
        {
            temp_StartConversion(power); // Set t_state to CONVERTING_T
        }
        break;
    case CONVERSION_DONE:
        presence = temp_Reset();
        if (presence)
        {
            TEMP = temp_ReadTemperature(); // Set t_state to STANDBY
        }
        break;
    case READING_ERROR:
        // Do nothing
        break;
    default:
        break;
    }

    int led = (int) (LED_MAX/(MAX_TEMP - MIN_TEMP) *( TEMP - MIN_TEMP));
    if(led != 0 && temp_CheckState() == STANDBY && TEMP != 85.0 && TEMP != -1.0){
        updateLedBar(led);

        unsigned char str[8];
        UART_parseTXData(str,TEMP,calefactor);
        UARTSendArray(str, 8);

        temp_int = ((int) TEMP);
    }

    if (UART_connection())
    {
        statusLed_PC(NORMAL);

        UART_RX_Buffer *rxBufferPointer = UART_getBuffer();
        UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
        temp_setTMuestreo(intMuestreo);

        if (!EEPROM_getWritingFlag())
        {
            // Parse data to be written
            EEPROM_parseData(WRITE, envio, LARGO_ENV, &setpoint, &histeresis, &intMuestreo);
            // Write data to EEPROM
            EEPROM_writeData(direccion, envio, LARGO_ENV);
        }
    }

    if(temp_int > setpoint + histeresis){
        PWM_setDC(10);
        calefactor = OFF;
    }else if(temp_int < setpoint - histeresis){
        PWM_setDC(90);
        calefactor = ON;
    }



}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
