/**
 * @file     encoder.h
 * @brief    Header del controlador de los LEDs de la placa del display
 * @author   Grupo 5
 */

#ifndef _LEDS_H_
#define _LEDS_H_

/* INCLUDE HEADER FILES */
/* Incluir archivos de encabezado */

#include "common.h"

/* CONSTANT AND MACRO DEFINITIONS USING #DEFINE */
/* DEFINICIONES CONSTANTES Y MACROS UTILIZANDO #DEFINE */

/* ENUMERATIONS AND STRUCTURES AND TYPEDEFS */
/* ENUMERACIONES, ESTRUCTURAS Y TIPOS DE DATOS */

enum on_off {OFF, ON};

/* VARIABLES WITH GLOBAL SCOPE 
 * +ej: unsigned int anio_actual;*/
/* VARIABLES CON ALCANCE GLOBAL
 * +ej: unsigned int anio_actual;*/


/* FUNCTION PROTOTYPES WITH GLOBAL SCOPE */
/* PROTOTIPOS DE FUNCIONES CON ALCANCE GLOBAL */

/**
 * @brief Inicia el controlador de los LEDs
 * @param mistakes Cantidad de errores iniciales
 */
void ledsInit(uint8_t mistakes);

/**
 * @brief Establece el estado de los LEDs
 * @param mistakes Cantidad de errores
 */
void setLeds(uint8_t mistakes);

#endif // _LEDS_H_
