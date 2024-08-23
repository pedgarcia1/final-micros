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
#define START_BYTE 0x02
#define STOP_BYTE 0x03


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
#define BUFFER_SIZE 128  // Tamaño del buffer circular

typedef struct {
    uint8_t data[BUFFER_SIZE];  // Array para almacenar los datos recibidos
    uint8_t head;               // Índice del próximo byte a escribir
    uint8_t tail;               // Índice del próximo byte a leer
    uint8_t count;              // Número de bytes en el buffer
    uint8_t receiving;          // Bandera de estado de recepción
    uint8_t rx_flag;            // Bandera para indicar que un paquete completo ha sido recibido
} UART_RX_Buffer;

typedef struct {
    unsigned char *str;   // Array para almacenar los datos recibidos
    unsigned char length;       // ï¿½ndice para rastrear la posiciÃ³n actual en el array
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

void UART_parseData(UART_RX_Buffer* buffer, uint8_t* setpoint, uint8_t* histeresis, uint16_t* data3);

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

uint8_t UARTReadByte(void);

void UART_parseTXData(unsigned char *str, float TEMP, uint8_t calefactor);
/*******************************************************************************
 ******************************************************************************/

#endif // _UART_H_
