/*******************************************************************************
  @file   74hc595.h
  @brief  Driver for 74HC595 shift register
          ONLY LATCH,CLOCK and DATA pin required (defined in board.h)
              OE pin set to LOW
              MR pin set to HIGH
  @author Pedro Garcia Delucis
 ******************************************************************************/

#ifndef LED_BAR_H
#define LED_BAR_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "74hc595.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief: Initializes Led Bar Display
*/
void ledBarInit();

/**
 * @brief: Sends multiple bytes of data
 * @param value: Number of bytes to be sent
*/
void updateLedBar(unsigned int level);

void statusLed_init(void);

void statusLed_setPeriod(unsigned int period);

/*******************************************************************************
 ******************************************************************************/

#endif // LED_BAR_H

