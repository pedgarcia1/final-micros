/***************************************************************************//**
  @file     ADC.h
  @brief    Header del driver de ADC
  @author   Grupo 5
 ******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_
#define VMAX 3660
#define VMIN 0

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "board.h"

/**
 * @brief Inicializa el ADC
*/
void adcInit(void);

/**
 * @brief Lee el ADC
*/
void readADC(void);

/**
 * @brief Convierte el valor del ADC a voltaje
*/
void adc2voltage(void);

/**
 * @brief Devuelve el valor de la lectura del ADC en voltaje
*/
uint16_t getVoltage(void);

/**
 * @brief Devuelve el valor del ADC en bits
*/
uint16_t getValue(void);

#endif // _ADC_H_
