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
void AppRun(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static const uint8_t power = 1; // 0 parasite power, 1 external power supply (Vdd)
uint8_t setpoint, histeresis, temp_int;
uint16_t intMuestreo;
uint8_t calefactor = 0;

uint8_t envio[LARGO_ENV] = {0xFA, 0xFB, 0xFC, 0xF0, 0xF2, 0xF4};
uint8_t direccion = 0x75; // Random Addres
uint8_t datos_leidos[LARGO_READ];
uint8_t debug;

#define MAX_TEMP    60.0
#define MIN_TEMP    20.0
#define LED_MAX     8.0

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
    // Inicializacin (se ejecuta 1 sola vez al comienzo)
    timerInitialization(TIMER_PERIOD); // 100ms timer perdios for ADC interrupt
    timerStart();
    temp_Init(); // Inicializa el sensor de temperatura DS18B20
    ledBarInit(); // Inicializa el display de 8 leds y el shift register 74HC595
    UART_init(NO_PERIODIC);
    PWM_Init();
    statusLed_init();
    EEPROM_init();
    // temp_SetResolution(TEMP_9_BIT); NO FUNCIONA, BORRAR O PROBAR CON EL DS18B20

    // Valores para debug
    setpoint = 30;
    histeresis = 1;
    intMuestreo = 1000;

    uint8_t timeout = 1;
    while (!timeout)
    {
        if (UART_connection())
        {                               // Chequea conexcion con PC por UART
            statusLed_setState(NORMAL); // LED estado normal

            // Lectura datos de la PC
            UART_RX_Buffer *rxBufferPointer = UART_getBuffer();
            UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
            temp_setTMuestreo(intMuestreo);
            timeout = 1; // Salir del loop
        }
    }

    if (!UART_connection())
    {
        statusLed_setState(ERROR); // LED estado error

        // Lectura de la EEPROM
        EEPROM_readData(direccion, datos_leidos, LARGO_READ);
        // Parse EEPROM data
        // if checksum OK
        setpoint = datos_leidos[0];
        histeresis = datos_leidos[1];
        // datos_leidos[2] MSB de intMuestreo
        // datos_leidos[3] LSB de intMuestreo
        intMuestreo = (datos_leidos[2] << 8) | datos_leidos[3];
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
        statusLed_setState(NORMAL);

        UART_RX_Buffer *rxBufferPointer = UART_getBuffer();
        UART_parseData(rxBufferPointer, &setpoint, &histeresis, &intMuestreo);
        temp_setTMuestreo(intMuestreo);

        if (!EEPROM_getWritingFlag())
        {
            envio[0] = setpoint;
            envio[1] = histeresis;
            envio[2] = (intMuestreo >> 8) & 0xFF;
            envio[3] = intMuestreo & 0xFF;
            envio[4] = 0x00;
            envio[5] = 0x00;
            // Write data to EEPROM
            EEPROM_writeData(direccion, envio, LARGO_ENV);
        }
    }
    else
    {
        // statusLed_setState(ERROR);
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
