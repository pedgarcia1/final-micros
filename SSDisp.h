/***************************************************************************//**
  @file     SSDisp.h
  @brief    Header del controlador del display de siete segmentos (Seven Segment Driver)
  @Author   Tristan Gonzalez Branca
 ******************************************************************************/

#ifndef _SSDisp_H_
#define _SSDisp_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"
#include <math.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLINK_SPEED 500
#define BLINK_LIMIT 32768
// Velocidad de parpadeo y l�mite para el parpadeo del d�gito activo

#define BLINK 0
#define STATIC 1

#define DISPLAY_DIGITS 4
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
 * @brief Imprime un d�gito en una posici�n espec�fica
 * @param pos La posici�n del d�gito a imprimir
 */
void printDigit(uint8_t pos);

/**
 * @brief Muestra el mensaje "OPEN" en el display
 */
void displayOpen(void);

/**
 * @brief Establece los valores a mostrar en el display
 * @param show[] Arreglo de 4 elementos con los valores a mostrar en cada posici�n
 */
void setDisplay(uint8_t show[]);

/**
 * @brief Establece el d�gito activo en el display
 * @param active La posici�n del d�gito a activar
 */
void setActive(uint8_t active);

/**
 * @brief Rutina de interrupci�n para el control del display con parpadeo del digito activo
 */
void displayBlinkISR(void);

/**
 * @brief Rutina de interrupci�n para el control del display sin parpadeo
 */
void displayStaticISR(void);

/**
 * @brief Inicializa el controlador del display de siete segmentos
 * @param selected_mode Indica el modo en el que se va a utilizar el display. Con un d�gito activo (BLINK) o sin (STATIC)
 */
void displayInit(uint8_t selected_mode);

/**
 * @brief Apaga el d�gito en la posici�n especificada
 * @param pos La posici�n del d�gito a apagar
 */
void printOff(uint8_t pos);

/**
 * @brief Muestra el mensaje "LOCK" en el display
 */
void displayLocked(void);

/**
 * @brief Convierte un d�gito decimal en su valor hexadecimal correspondiente
 * @param val El d�gito decimal
 * @param pos La posici�n en la que se almacenar� el valor hexadecimal
 */
void digit2hexa(uint8_t val, uint8_t pos);

/**
 * @brief Separa un numero float en 4 digitos significativos
 * @param value El numero float que se desea convertir
 */
void setDisplay_float(uint16_t value);
/*******************************************************************************
 ******************************************************************************/

#endif // _SSDisp_H_
