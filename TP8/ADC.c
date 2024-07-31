/***************************************************************************//**
  @file     ADC.c
  @brief    Driver de ADC
  @Author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "ADC.h" 
#include "msp430g2553.h"
#include "timer.h"
#include "isr.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define N_BITS 1024
#define FACTOR_ESCALA 3.2

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct 
{
    volatile uint16_t value;
    volatile uint16_t voltage;
}adc_t;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
adc_t adc_data;

void adcInit(void)
{
    /* Select Reference */
    ADC10CTL0 = SREF_0 | ADC10SHT_0 | ADC10ON;
  	//  ADC10CTL0 = SREF_1 + ADC10SHT_2 + ADC10ON + REF2_5V + REFON;


    /*Select input channel P1.7*/
    ADC10CTL1 = INCH_7; // input A7

    /* Enable  P1.7 pin as Analog innput (A7) */
    ADC10AE0 = ADC_PIN; // P1.7 ADC option select

	send_to_timer_isr(readADC, 1);
}

void readADC(void)
{

	ADC10CTL0 |= ENC + ADC10SC; // Enable converter & Start of conversion
	// chequear esta parte de busy
	while (ADC10CTL1 & BUSY)
	{ /* do nothing */ } // Wait if ADC10 core is active
	adc_data.value=ADC10MEM;  // Read adc value
	adc2voltage();
	// return adc_data.value;
	timerFlagClear();

}

void adc2voltage(void){
	// Conversion a voltaje
    adc_data.voltage = (uint16_t) (FACTOR_ESCALA * adc_data.value);
}

uint16_t getVoltage(void){
	// Devolver el valor de la lectura del ADC en voltaje
	return adc_data.voltage;
}

uint16_t getValue(void){
	// Devolver el valor del ADC en bits
	return adc_data.value;
}
