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
#include "SSDisp.h"
#include "hardware.h"
#include "ADC.h"
#include "boardleds.h"
#include "encoder.h"
#include "drv_UART.h"
#include "timer.h"


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
uint16_t voltage,value;
uint8_t lightState;

uint8_t write_mode = 'C';
unsigned char tx_message[5];
unsigned char rcv_message;
uint8_t rxFlag;
uint8_t transmitterFlag;

uint8_t dummy = 0;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void AppInit(void);
void AppRun(void);
void getDecStr (uint8_t* str, uint8_t len, uint32_t val,uint8_t n);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
#define EJERCICIO 3

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



void main(void)
{
    // NO TOCAR
    systemInitFirst();
    boardInit();
    AppInit();
    systemInitLast();

    for (;;)
        AppRun();
    // NO TOCAR
}

#if EJERCICIO == 1


void AppInit(void)
{
    // Inicializaci�n (se ejecuta 1 sola vez al comienzo)
    // Inicializaci�n del display
    displayInit(STATIC);
    adcInit();
    timerInitialization(13100); // 100ms timer perdios for ADC interrupt
    timerStart();
    
}

void AppRun(void)
{
    // Loop (se ejecuta constantemente en un ciclo infinito)
    voltage = getVoltage();

    setDisplay_float(voltage);

}

#elif EJERCICIO == 2
#define UPPER_VOLTAGE 2
#define LOWER_VOLTAGE 1.3

void AppInit(void)
{
    // Inicializaci�n (se ejecuta 1 sola vez al comienzo)
    // Inicializaci�n del display
    displayInit(STATIC);
    adcInit();
    timerInitialization(TIMER_PERIOD); // 100ms timer perdios for ADC interrupt
    timerStart();
    ledsInit(OFF);
    lightState = OFF;
}   

void AppRun(void) // Loop (se ejecuta constantemente en un ciclo infinito)
{
    voltage = getVoltage();

    setDisplay_float(voltage);

    if((lightState == OFF) && (voltage >= UPPER_VOLTAGE))
    {
        lightState = ON;
        setLeds(ON);
    }

    if ((lightState == ON) && (voltage <= LOWER_VOLTAGE))
    {
        lightState = OFF;
        setLeds(OFF);
    }
}

#elif EJERCICIO == 3

#define UPPER_VOLTAGE 2
#define LOWER_VOLTAGE 1


void AppInit(void)
{
    // Inicializaci�n (se ejecuta 1 sola vez al comienzo)
    // Inicializaci�n del display
    displayInit(STATIC);
    encoderInit();
    adcInit();
    UART_init(START_PERIODIC);
    timerInitialization(TIMER_PERIOD); // 100ms timer perdios for ADC interrupt
    timerStart();
    ledsInit(OFF);
    lightState = OFF;
    tx_message[5] = '\0';
    
}   

void AppRun(void) // Loop (se ejecuta constantemente en un ciclo infinito)
{

    voltage = getVoltage();
    value = getValue();

    setDisplay_float(voltage);

    if (lightState == OFF && (((uint8_t)(voltage / 1000)) < LOWER_VOLTAGE))
    {
        lightState = ON;
        ledsInit(ON);
    }
    else if (lightState == ON &&  (((uint8_t)(voltage / 1000)) > UPPER_VOLTAGE))
    {
        lightState = OFF;
        ledsInit(OFF);
    }

    
    // Verificar si hubo un mensaje
    rxFlag = getRXStatus();


    if (rxFlag > 0){
            rcv_message = getChar();
            resetRXStatus();
            if(rcv_message == 'C' || rcv_message == 'V'){
                if(rcv_message=='C'){
                    UARTSendArray("Modo Raw",8);
                    UARTSendArray("\n\r", 2);
                }
                else{
                    UARTSendArray("Modo Volt",9);
                    UARTSendArray("\n\r", 2);
                }
                write_mode = rcv_message;
            }
        }

    // Enviar el mensaje

    if (write_mode == 'C'){
        //int2ASCII(value);
        getDecStr(tx_message, 4, value,0);
        setTXMessage(tx_message, 5);

    }else{
        getDecStr(tx_message, 5, voltage,2);
        setTXMessage(tx_message, 6);

    }
    //setTXMessage(tx_message, 6);

    // Logica segun estado del encoder
    
    switch (encoderGetStatus()) {
        case CW:
            // El encoder gira en sentido horario
            encoderResetStatus(); // Reinicia el estado del encoder
            incrementUARTPeriod();
            break;

        case CCW:
            // El encoder gira en sentido antihorario
            encoderResetStatus(); // Reinicia el estado del encoder
            decrementUARTPeriod();
            break;

    }

}

void getDecStr (uint8_t* str, uint8_t len, uint32_t val,uint8_t n)
{
  uint8_t i;

  for(i=1; i<=len; i++)
  {
    if(i==len-n+1 & n!=0){
        str[len-i]='.';}
    else{
        str[len-i] = (uint8_t) ((val % 10UL) + '0');
        val/=10;
    }
  }

  str[i-1] = '\0';
}

#endif // EJERCICIO


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



