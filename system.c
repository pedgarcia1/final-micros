/***************************************************************************//**
  @file     system.c
  @brief    Controlador del sistema del MCU
  @Author
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "system.h"

#include "board.h"
#include "hardware.h"

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
 * @brief Inicializa el sistema del MCU.
 *        Configura la frecuencia del DCO (Oscilador Controlado Digitalmente) y establece las fuentes y preescaladores de MCLK y SMCLK.
 */
void systemInitFirst(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Detener el temporizador de watchdog

    // Configurar DCO
    DCOCTL = 0;
#if DCOCLK_FREQ_HZ == 1000000UL
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
#elif DCOCLK_FREQ_HZ == 8000000UL
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
#elif DCOCLK_FREQ_HZ == 12000000UL
    BCSCTL1 = CALBC1_12MHZ;
    DCOCTL = CALDCO_12MHZ;
#elif DCOCLK_FREQ_HZ == 16000000UL
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
#else
#error Mal definido el clock del sistema
#endif

    // Configurar MCLK y SMCLK
    BCSCTL2 = SELM_0 | (MCLK_PRESCALER_2POW << 4) | (SMCLK_PRESCALER_2POW << 4);
}

/**
 * @brief Inicializa los perif�ricos y configuraciones de la placa.
 *        TODO: Completar con la configuraci�n de los pines restantes utilizados.
 */
void boardInit(void)
{
    // TODO: COMPLETAR CON LA CONFIGURACI�N DE LOS PINES RESTANTES UTILIZADOS
    P2SEL &= ~BIT6; // Pines 2.6 y 2.7 en funcionalidad gpio
    P2SEL &= ~BIT7;
}

/**
 * @brief Realiza los �ltimos pasos de inicializaci�n del sistema del MCU.
 *        Configura el intervalo del temporizador de watchdog y habilita la interrupci�n.
 */
void systemInitLast(void)
{
    WDTCTL = WDT_MDLY_0_5; // Modo de temporizador de intervalo; seg�n la selecci�n del intervalo
    IE1 |= WDTIE; // Habilitar el bit WDTIE
    enable_interrupts(); // Habilitar interrupciones generales
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/

