/***************************************************************************//**
  @file     SSDisp.h
  @brief    Seven Segment Driver
  @author   Tristan Gonzalez Branca
 ******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define START_PERIODIC 1
#define NO_PERIODIC 0
#define BUFFER_SIZE 10
#define START_BYTE 0x02
#define STOP_BYTE 0x03


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    uint8_t data[BUFFER_SIZE];   // Array para almacenar los datos recibidos
    uint8_t index;       // �ndice para rastrear la posición actual en el array
    uint8_t receiving;   // Flag para indicar si se está recibiendo datos
    uint8_t rx_flag;     // Flag para indicar que se recibieron datos
} UART_RX_Buffer;

typedef struct {
    unsigned char *str;   // Array para almacenar los datos recibidos
    unsigned char length;       // �ndice para rastrear la posición actual en el array
    uint8_t transmiting;
} UART_TX_Buffer;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
* @brief Inicializa el UART
*/
void UART_init(uint8_t periodic_flag);

/**
 * @brief Envia Array por UART
 * @param *TxArray Array a enviar
 * @param ArrayLength Largo del array
 */
void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength);

/**
 * @brief Devuelve el puntero al buffer de recepcion
*/
UART_RX_Buffer* UART_getBuffer();

/**
 * @brief Devuelve el caracter recibido
*/
uint8_t getChar();

void UART_parseData(UART_RX_Buffer* buffer, uint8_t* data1, uint8_t* data2, uint8_t* data3);

uint8_t UART_connection(void);

/**
 * @brief Setea el periodo de trabajo de UART
 * @param period Periodo de trabajo
*/
void setUARTPeriod(uint16_t period);

/**
* @brief Funcion periodica de UART
*/
void UARTPeriodic();

/**
 * @brief Setea el mensaje a enviar por UART
 * @param Text Mensaje a enviar
 * @param Largo Largo del mensaje
*/
void setTXMessage(unsigned char *Text, unsigned char Largo);

/**
 * @brief Incrementa el periodo de trabajo de UART en 100 ms
*/
void incrementUARTPeriod();

/**
 * @brief Decrementa el periodo de trabajo de UART en 100 ms
*/
void decrementUARTPeriod();
/*******************************************************************************
 ******************************************************************************/

#endif // _UART_H_
