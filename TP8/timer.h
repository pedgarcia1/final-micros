/***************************************************************************//**
  @file     timer.h
  @brief    Header del driver del TIMER_A0 driver para el MSP430G2553
  @author   Grupo 5
 ******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "msp430.h"
#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*Timer Useful definitions*/

/* Timer Counter Modes */
#define TIMER_STOP       MC_0   // Stop
#define TIMER_UP         MC_1   // Up to CCR0
#define TIMER_CONTINUOUS MC_2   // Up to 0xFFFF
#define TIMER_UPDOWN     MC_3   // Up to CCR0 Down to 0
#define TIMER_PERIOD     12499


/**
 * @brief Inicializa el timer
 * @param period Periodo de la interrupcion
*/
void timerInitialization(uint16_t period);


/**
 * @brief Comienza el timer
 */
void timerStart(void);


/**
 * @brief Detiene el timer
 */
void timerStop(void);


/**
 * @brief Limpia el flag de interrupcion del timer
 */
void timerFlagClear(void);


#endif // _TIMER_H_
