
/**
 * @file     encoder.h
 * @brief    Controlador de los LEDs de la placa del display
 * @author   Grupo 5
 */


/** INCLUDE HEADER FILES **/

#include "boardleds.h"
#include "gpio.h"
#include "board.h"

/* CONSTANT AND MACRO DEFINITIONS USING #DEFINE */



/* ENUMERATIONS AND STRUCTURES AND TYPEDEFS */



/* VARIABLES WITH GLOBAL SCOPE 
 * +ej: unsigned int anio_actual;*/


/* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 * +ej: static void falta_envido (int);+ */


/** ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 * +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+ */


/* GLOBAL FUNCTION DEFINITIONS */

void ledsInit(uint8_t mistakes){
    gpioMode(Status0, OUTPUT);
    gpioMode(Status1, OUTPUT);
    setLeds(mistakes);
}

void setLeds(uint8_t mistakes)
{
    if(mistakes == 0)
    {
        gpioWrite(Status0,LOW);
        gpioWrite(Status1,LOW);
    }
    else if(mistakes == 1)
    {
        gpioWrite(Status0,HIGH);
        gpioWrite(Status1,HIGH);
    }
    else if(mistakes == 2)
    {
        gpioWrite(Status0,LOW);
        gpioWrite(Status1,HIGH);
    }else if(mistakes == 3)
    {
        gpioWrite(Status0,HIGH);
        gpioWrite(Status1,LOW);
    }
}

/* LOCAL FUNCTION DEFINITIONS */


 

