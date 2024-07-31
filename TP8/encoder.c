/**
 * @file     encoder.c
 * @brief    Controlador del encoder
 * @Author   Teo Nicoletti
 */

/** INCLUDE HEADER FILES **/

#include "encoder.h"
#include "gpio.h"
#include "board.h"
#include "isr.h"

/* CONSTANT AND MACRO DEFINITIONS USING #DEFINE */

/* ENUMERATIONS AND STRUCTURES AND TYPEDEFS */

/* VARIABLES WITH GLOBAL SCOPE 
 * +ej: unsigned int anio_actual;*/
encoderStatus_t status;


/* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 * +ej: static void falta_envido (int);+ */



/** ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 * +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+ */

volatile uint8_t A_now, A_before, B_now, B_before;


/** STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
// +ej: static int temperaturas_actuales[4];+ */



/* GLOBAL FUNCTION DEFINITIONS */

/**
 * @brief Inicializa el controlador del encoder
 */
void encoderInit(void){
    gpioMode(ENC_A_PIN, INPUT_PULLUP);
    gpioMode(ENC_B_PIN, INPUT_PULLUP);
    send_to_isr(encoderISR, 10);
}

/**
 * @brief Obtiene el estado actual del encoder
 * @return El estado actual del encoder
 */
encoderStatus_t encoderGetStatus(){
    return status;
}

/**
 * @brief Restablece el estado del encoder
 */
void encoderResetStatus(){
    status = IDLE;
}

/**
 * @brief Rutina de interrupción del encoder
 * @return El estado actual del encoder
 */
void encoderISR() {
    if (status == IDLE){
        A_now = gpioRead(ENC_A_PIN);
        B_now = gpioRead(ENC_B_PIN);

        if ((A_now ^ 1) & A_before & B_now & B_before){
            status = CCW;
        }
        else if ((B_now ^ 1) & B_before & A_now & A_before) {
            status = CW;
        }
    }
    A_before = A_now;
    B_before = B_now;
}

/* LOCAL FUNCTION DEFINITIONS */
