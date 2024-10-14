
#ifndef PWM_H_
#define PWM_H_


#include "board.h"
#include "timer.h"

uint8_t duty_cycle;

enum {
    OFF,
    ON
};
/*
 Prototypes
 */
void PWM_Init(void);

void PWM_setDC(uint8_t value);

#endif /* PWM_H_ */

