/**
 * @file     encoder.h
 * @brief    Archivo de encabezado del controlador del encoder
 * @Author   Teo Nicoletti
 */

#ifndef _ENCODER_H_
#define _ENCODER_H_

/* INCLUDE HEADER FILES */

#include "common.h"

/* CONSTANT AND MACRO DEFINITIONS USING #DEFINE */

/* ENUMERATIONS AND STRUCTURES AND TYPEDEFS */

/**
 * @brief Enumeración de estados del encoder
 */
typedef enum {
    IDLE, /**< Estado inactivo, no se detectó movimiento */
    CW,   /**< Movimiento en sentido horario detectado */
    CCW   /**< Movimiento en sentido antihorario detectado */
} encoderStatus_t;

/* VARIABLES WITH GLOBAL SCOPE 
 * +ej: unsigned int anio_actual;*/

/* FUNCTION PROTOTYPES WITH GLOBAL SCOPE */

/**
 * @brief Inicializa el controlador del encoder
 */
void encoderInit(void);

/**
 * @brief Obtiene el estado actual del encoder
 * @return El estado del encoder:
 *         - IDLE (0) si no se ha producido movimiento
 *         - CW (1) si se ha producido un movimiento en sentido horario
 *         - CCW (2) si se ha producido un movimiento en sentido antihorario
 */
void encoderISR(void);

/**
 * @brief Obtiene el estado actual del encoder
 * @return El estado del encoder:
 *         - IDLE (0) si no se ha producido movimiento
 *         - CW (1) si se ha producido un movimiento en sentido horario
 *         - CCW (2) si se ha producido un movimiento en sentido antihorario
 */
encoderStatus_t encoderGetStatus(void);

/**
 * @brief Restablece el estado del encoder
 */
void encoderResetStatus(void);



#endif // _ENCODER_H_
