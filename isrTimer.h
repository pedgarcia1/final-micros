/***************************************************************************//**
  @file     timer.h
  @brief    Header del driver del TIMER_A0 driver para el MSP430G2553
  @author   Grupo 5
 ******************************************************************************/

#ifndef _ISR_TIMER_H_
#define _ISR_TIMER_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "msp430.h"
#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

void timerTimeoutStart(unsigned int period);

void timerTimeoutStop(void);

uint8_t timerGetTimeout(void);

#endif // _ISR_TIMER_H_
