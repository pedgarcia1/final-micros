/***************************************************************************//**
  @file     gpio.h
  @brief    Header de servicios simples de pines GPIO, similares a Arduino
  @author   Grupo 5
 ******************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LOW             0
#define HIGH            1

#ifndef INPUT
#define INPUT           0
#define OUTPUT          1
#define INPUT_PULLUP    2
#define INPUT_PULLDOWN  3
#endif // INPUT


// Convertir el puerto y el número en identificador de pin
// Ej: P1.4 -> PORTNUM2PIN(1, 4) --> 0x14
//     P2.6 -> PORTNUM2PIN(2, 6) --> 0x26
#define PORTNUM2PIN(po, n)  ((po)<<4 | (n))
#define PIN2PORT(pi)        (((pi)>>4) & 0x03)
#define PIN2NUM(pi)         ((pi) & 0x07)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t gpio_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configura el pin especificado para comportarse como un GPIO
 * @param pin Identificador del pin (según PORTNUM2PIN)
 * @param mode Modo deseado (INPUT, OUTPUT, INPUT_PULLUP o INPUT_PULLDOWN)
 */
void gpioMode(gpio_t pin, uint8_t mode);

/**
 * @brief Escribe un valor HIGH o LOW en un pin digital
 * @param pin Identificador del pin (según PORTNUM2PIN)
 * @param value Valor deseado (HIGH o LOW)
 */
void gpioWrite(gpio_t pin, uint8_t value);

/**
 * @brief Invierte el valor de un pin digital (HIGH<->LOW)
 * @param pin Identificador del pin (según PORTNUM2PIN)
 */
void gpioToggle(gpio_t pin);

/**
 * @brief Lee el valor de un pin digital, ya sea HIGH o LOW
 * @param pin Identificador del pin (según PORTNUM2PIN)
 * @return HIGH o LOW
 */
uint8_t gpioRead(gpio_t pin);


/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
