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

    // Valores para debug
    setpoint = 30;
    histeresis = 1;
    intMuestreo = 1;
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

    int led = (int) (0.8*TEMP-15.0);
    if(led != 0 && temp_CheckState() == STANDBY && TEMP != 85.0 && TEMP != -1.0){
        updateLedBar(led);

        unsigned char str[8];
        UART_parseTXData(str,TEMP,calefactor);
        UARTSendArray(str, 8);

        temp_int = ((int) TEMP);
    }

    if(UART_connection()){
        statusLed_setState(NORMAL);

        UART_RX_Buffer* rxBufferPointer = UART_getBuffer();
        UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
    }else{
        statusLed_setState(ERROR);


    }

    uint8_t setpoint_anterior = setpoint;
    if(setpoint_anterior != setpoint){
        uint8_t guardar;
        guardar = setpoint - 1;
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
