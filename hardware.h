/***************************************************************************//**
  @file     hardware.h
  @brief    Hardware, system and MCU registers management
  @author   Grupo 5
 ******************************************************************************/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"

#include "msp430.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** SYSTEM defines *********************************************************/

#define DCOCLK_FREQ_HZ          1000000UL // 8MHz
#define MCLK_PRESCALER_2POW     0
#define SMCLK_PRESCALER_2POW    0
#define MCLK_FREQ_HZ            (DCOCLK_FREQ_HZ/(1UL<<MCLK_PRESCALER_2POW)) // 8MHz
#define SMCLK_FREQ_HZ           (DCOCLK_FREQ_HZ/(1UL<<SMCLK_PRESCALER_2POW)) // 8MHz
#define DELAY_SEC               5 // Segundos de bloquo de display cuando se alcanza la cantidad maxima de errores

#define enable_interrupts()     _BIS_SR(GIE)
#define disable_interrupts()    _BIC_SR(GIE)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
    // Se definen cantidad de ciclos de delay segun frecuencia del clock y segun DELAY_SEC
    #if DCOCLK_FREQ_HZ == 1000000UL
        #define DELAY_CYCLES 1000000*DELAY_SEC/3
    #elif DCOCLK_FREQ_HZ == 8000000UL
        #define DELAY_CYCLES 8000000*DELAY_SEC/3
    #elif DCOCLK_FREQ_HZ == 12000000UL
        #define DELAY_CYCLES 12000000*DELAY_SEC/3
    #elif DCOCLK_FREQ_HZ == 61000000UL
        #define DELAY_CYCLES 61000000*DELAY_SEC/3
    #else
    #endif

/*******************************************************************************
 ******************************************************************************/

#endif // _HARDWARE_H_
