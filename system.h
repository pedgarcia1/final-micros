/***************************************************************************//**
  @file     system.h
  @brief    Header del controlador del sistema del MCU
  @Author   Nicolás Magliola
 ******************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CLOSED 0
#define OPEN 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
    uint8_t psw_try[4];
    uint8_t active;

} display_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa el sistema en primer lugar: aspectos internos del MCU (reloj, watchdog, etc.)
 */
void systemInitFirst(void);


/**
 * @brief Inicializa la placa: aspectos de interfaz MCU-placa (distribución de pines, etc.)
 */
void boardInit(void);


/**
 * @brief Inicializa el sistema al final: aspectos internos del MCU (ISR, etc.)
 */
void systemInitLast(void);


/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTEM_H_
