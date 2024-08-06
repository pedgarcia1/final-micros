/*******************************************************************************
  @file     I2C_MSP430.c
  @brief    I2C communication driver.
            - PIN 1.6 -> SCL (UCB0SCL)
            - PIN 1.7 -> SDA (UCB0SDA)
            - Connect both pins with a pull-up resistor (10k) to Vcc (not larger than MSP430 Vcc)
  @author   Ignacio Quintana
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "74hc595.h"
#include "board.h"

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

/**
 * @brief:
*/
void shiftRegisterClock();

/**
 * Get a single bit from a byte.
 *
 * @param b A byte.
 * @param pos The bit position to be extracted.
 * @return A bit.
 */
unsigned int get_byte(char b, unsigned int pos);

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
void shiftRegisterInit() {
    // Set pins as output
    //P1DIR |= DATA + LATCH + CLOCK;
    gpioMode(DATA, OUTPUT);
    gpioMode(LATCH, OUTPUT);
    gpioMode(CLOCK, OUTPUT);
}

/**
 * @brief: Sends a byte of data
 * @param value: Data to be sent
*/
void shiftRegisterWrite(unsigned char value)
{
    // Latch LOW to start shifting data.
	// P1OUT &= ~LATCH;
    gpioWrite(LATCH, LOW);

	unsigned int i;
	for (i = 0; i < 8; i++) {
		// Shift every bit.
		// bit_to_pin(value, i, &P1OUT, DATA);
		if (get_byte(value, i)) {
                gpioWrite(DATA, HIGH);
		    } else {
                gpioWrite(DATA, LOW);
		    }
		shiftRegisterClock();
	}

	// Latch the data.
    gpioWrite(LATCH, HIGH);
    gpioWrite(LATCH, LOW);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief:
*/
void shiftRegisterClock() {
    // Set pins as output
    //P1DIR |= DATA + LATCH + CLOCK;
    gpioWrite(CLOCK, HIGH);
    gpioWrite(CLOCK, LOW);
}

/**
 * Get a single bit from a byte.
 *
 * @param b A byte.
 * @param pos The bit position to be extracted.
 * @return A bit.
 */
unsigned int get_byte(char b, unsigned int pos) {
	return (b & (1 << pos));
}
