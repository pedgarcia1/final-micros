/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// UART
// P1.1 and P1.2 in use

// DS18B20
#define ONE_WIRE 1,0

// Shift register
#define DATA PORTNUM2PIN(1,4)
#define LATCH PORTNUM2PIN(1,5)
#define CLOCK PORTNUM2PIN(1,3)

// PWM Pin
#define PWM_PIN BIT2 // P2.2

#define STATUS_LED PORTNUM2PIN(2,1) // P2.1



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
