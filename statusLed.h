
#ifndef STATUS_LED_H_
#define STATUS_LED_H_

/*
 * Enum
 */
typedef enum {
    ERROR = 0,
    NORMAL = 1
} status;

/*
 Prototypes
 */
void statusLed_init(void);

void statusLed_setPeriod(unsigned int period);

void statusLed_setState(status value);

#endif /* STATUS_LED_H_ */
