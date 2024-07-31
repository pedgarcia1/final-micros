/***************************************************************************//**
  @file     isr.h
  @brief    Header del driver de interrupciones
  @author   Grupo 5
 ******************************************************************************/

#ifndef _ISR_H_
#define _ISR_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/




/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: Declarar prototipos de variables globales aqu� (ej: extern unsigned int anio_actual;)+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Env�a una funci�n para que se ejecute en la rutina de servicio de interrupci�n
 * @param function Puntero a la funci�n que se ejecutar�
 * @param period Per�odo de ejecuci�n de la funci�n en n�mero de interrupciones (ej: 1 = en cada interrupci�n, 2 = en cada 2 interrupciones, etc.)
 */
void send_to_isr(void (*function)(void), unsigned int period);

/**
 * @brief Env�a una funci�n para que se ejecute en la rutina de servicio de interrupci�n del timer A
 * @param function Puntero a la funci�n que se ejecutar�
 * @param period Per�odo de ejecuci�n de la funci�n en n�mero de interrupciones (ej: 1 = en cada interrupci�n, 2 = en cada 2 interrupciones, etc.)
 */
void send_to_timer_isr (void(*function)(void), unsigned int period);

// +ej: Declarar prototipos de funciones globales aqu� (ej: char lcd_goto (int fil, int col);)+


/*******************************************************************************
 ******************************************************************************/

#endif // _ISR_H_
