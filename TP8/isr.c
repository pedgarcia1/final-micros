/***************************************************************************//**
  @file     isr.c
  @brief    Driver de interrupciones
  @Author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

// +Incluir los archivos de encabezado necesarios (ej: #include "template.h")+
#include "isr.h"
#include "hardware.h"
#include <stdlib.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LARGO_VECTOR_ISR 6

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Estructura de ISR (isr_t)
typedef struct
{
  void (*function_ptr)(void);
  unsigned int counter_reset;
  unsigned int counter;
} isr_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: Declarar variables globales aqu� (ej: unsigned int anio_actual;)+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: Declarar prototipos de funciones privadas aqu� (ej: static void falta_envido (int);)+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: Definir variables ROM const aqu� (ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};)+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: Definir variables est�ticas aqu� (ej: static int temperaturas_actuales[4];)+

isr_t isr_vector[LARGO_VECTOR_ISR];
static isr_t *vector_ptr = isr_vector;
static unsigned int length = 0;

isr_t timer_isr_vector[3];
static isr_t *timer_vector_ptr = timer_isr_vector;
static unsigned int length_timer = 0;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void send_to_isr (void(*function)(void), unsigned int period) {

    // A�adir la funci�n y su per�odo al vector de ISR

    // Chequeo si la función ya está en el vector y reemplazo los nuevos datos
    uint8_t i;
    for (i = 0; i < length; i++) {
        if (vector_ptr[i].function_ptr == function) {
            vector_ptr[i].counter_reset = period;
            vector_ptr[i].counter = period;
            return;
        }
    }
    // Chequeo si el vector esta lleno y vuelvo al inicio
    if (length == LARGO_VECTOR_ISR) {
        length = 0;
    }
    vector_ptr[length].function_ptr = function;
    vector_ptr[length].counter_reset = period;
    vector_ptr[length].counter = period;
    length++;
}

void send_to_timer_isr (void(*function)(void), unsigned int period) {

    // A�adir la funci�n y su per�odo al vector de ISR
    timer_vector_ptr[length_timer].function_ptr = function;
    timer_isr_vector[length_timer].counter_reset = period;
    timer_isr_vector[length_timer].counter = period;
    length_timer++;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void) {

    uint8_t i;

    // Iterar sobre el vector de ISR y ejecutar las funciones correspondientes
    for(i = 0; i<length; i++) {
        vector_ptr[i].counter--;
        if (!vector_ptr[i].counter) {
            vector_ptr[i].function_ptr();
            vector_ptr[i].counter = vector_ptr[i].counter_reset;
        }
    }
}


// Timer A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR        //Interrupt Service Routine (ISR) for CCR0 (only)
__interrupt void TIMER_ISR(void)
{

    uint8_t i;
    // Iterar sobre el vector de ISR y ejecutar las funciones correspondientes
    for(i = 0; i<length; i++) {
        timer_vector_ptr[i].counter--;
        if (!timer_vector_ptr[i].counter) {
            timer_vector_ptr[i].function_ptr();
            timer_vector_ptr[i].counter = timer_vector_ptr[i].counter_reset;
        }
    }

}
