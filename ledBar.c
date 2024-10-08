/*******************************************************************************
  @file     ledBar.c
  @brief
  @author
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "74hc595.h"
#include "ledBar.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
unsigned char set_led(unsigned int i);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief: Init. SR 74HC595
*/
void ledBarInit() {
    shiftRegisterInit();
    updateLedBar(0);
}

/**
 * @brief: Sends a byte of data
 * @param value: Data to be sent
*/
void updateLedBar(unsigned int level)
{
    unsigned char leds;
    leds = set_led(level);
    shiftRegisterWrite(leds);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

unsigned char set_led(unsigned int i) {
    unsigned char leds;

    switch (i) {
        case 0:
            leds = 0b00000000;  // Todos los LEDs apagados
            break;
        case 1:
            leds = 0b00000001;  // Primer LED encendido
            break;
        case 2:
            leds = 0b00000011;  // Primer y segundo LED encendidos
            break;
        case 3:
            leds = 0b00000111;  // Primer, segundo y tercer LED encendidos
            break;
        case 4:
            leds = 0b00001111;  // Primer, segundo, tercer y cuarto LED encendidos
            break;
        case 5:
            leds = 0b00011111;  // Primer, segundo, tercer, cuarto y quinto LED encendidos
            break;
        case 6:
            leds = 0b00111111;  // Primer, segundo, tercer, cuarto, quinto y sexto LED encendidos
            break;
        case 7:
            leds = 0b01111111;  // Primer, segundo, tercer, cuarto, quinto, sexto y séptimo LED encendidos
            break;
        case 8:
            leds = 0b11111111;  // Todos los LEDs encendidos
            break;
        default:
            leds = 0b00000000;  // Valor por defecto: todos los LEDs apagados
            break;
    }

    return leds;
}


