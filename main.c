/***************************************************************************//**
  @file     main.c
  @brief    Archivo principal del TP8 ej. 1
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
#include "statusLed.h"

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
void AppRun(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static const uint8_t power = 0; // 0 parasite power, 1 external power supply (Vdd)
uint8_t setpoint, histeresis, intMuestreo, temp_int;
uint8_t calefactor = 0;

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
    systemInitLast();

    for (;;)
        AppRun();
    // NO TOCAR
}

void AppInit(void)
{
    // Inicializaci�n (se ejecuta 1 sola vez al comienzo)
    timerInitialization(TIMER_PERIOD); // 100ms timer perdios for ADC interrupt
    timerStart();
    temp_Init(); // Inicializa el sensor de temperatura DS18B20
    ledBarInit(); // Inicializa el display de 8 leds y el shift register 74HC595
    UART_init(NO_PERIODIC);
    PWM_Init();
    statusLed_init();
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
        presence = temp_Reset();
        if (presence)
        {
            TEMP = temp_ReadTemperature(); // Set t_state to STANDBY
        }
    default:
        break;
    }

    int led = (int) (0.8*TEMP-15.0);
    if(led != 0 && temp_CheckState() == STANDBY && TEMP != 85.0){
        updateLedBar(led);

        temp_int = ((uint8_t) TEMP);
        unsigned char str[8];
        str[0] = temp_int / 10 + '0'; // primer digito
        str[1] = temp_int % 10 + '0'; // segundo digito
        str[2] = '.'; // punto decimal
        str[3] = (int) (TEMP * 10) % 10 + '0';  // primer decimal
        str[4] = (int) (TEMP * 100) % 10 + '0'; // segundo decimal
        str[5] = ','; // coma
        str[6] = calefactor + '0'; // estado del calefactor calefactor
        str[7] = '\n';
        UARTSendArray(str, 8);
    }

    if(UART_connection()){
        statusLed_setState(NORMAL);

        UART_RX_Buffer* rxBufferPointer = UART_getBuffer();
        UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
    }else{
        statusLed_setState(ERROR);

        setpoint = 30;
        histeresis = 2;
        intMuestreo = 0;
    }


    if(temp_int > setpoint + histeresis){
        PWM_setDC(10);
        calefactor = 0; // OFF
    }else if(temp_int < setpoint - histeresis){
        PWM_setDC(90);
        calefactor = 1; // ON
    }



}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// PASAR A ISR @TEO :) <3
// ISR del Watchdog Timer
/*
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    // 1 interrupcion de timer cada 0.5ms
    if (temp_CheckState() == CONVERTING_T)
    {
        count++;
        if (count >= 1000 * 2) // 1 interrupcion de timer cada 0.5ms
        {
            temp_SetState(CONVERSION_DONE);
            count = 0;
        }
    }
}
*/
