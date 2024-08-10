#ifndef _GPIO_OW_H_
#define _GPIO_OW_H_

// --------------- DEFINES --------------- //
/******** PinMode Macros ********/
// PinOut
#define PIN_OUTPUT(...)                         PIN_OUTPUT_SUB(__VA_ARGS__)
#define PIN_OUTPUT_SUB(PORT, PIN)               { P##PORT##DIR |=  (1 << PIN); }

// PinInput
#define PIN_INPUT(...)                          PIN_INPUT_SUB(__VA_ARGS__)
#define PIN_INPUT_SUB(PORT, PIN)                { P##PORT##DIR &= ~(1 << PIN); P##PORT##REN &= ~(1 << PIN); }

// PinSel
#define PIN_SEL(...)                            PIN_SEL_SUB(__VA_ARGS__)
#define PIN_SEL_SUB(PORT, PIN, STATUS)          { (STATUS==HIGH) ? (P##PORT##SEL  |= (1 << PIN)) : (P##PORT##SEL &= ~(1 << PIN)); }

// PinSel2
#define PIN_SEL2(...)                           PIN_SEL2_SUB(__VA_ARGS__)
#define PIN_SEL2_SUB(PORT, PIN, STATUS)         { (STATUS==HIGH) ? (P##PORT##SEL2 |= (1 << PIN)) : (P##PORT##SEL2 &= ~(1 << PIN)); }

// PinInput Pullup/Pulldown (Not Used)
// #define PIN_INPUT_PULLUP(PORT, PIN)          { P##PORT##DIR &= ~(1 << PIN); P##PORT##REN |=  (1 << PIN); P##PORT##OUT |=  (1 << PIN); }
// #define PIN_INPUT_PULLDOWN(PORT, PIN)        { P##PORT##DIR &= ~(1 << PIN); P##PORT##REN |=  (1 << PIN); P##PORT##OUT &= ~(1 << PIN); }

/******** PinWrite Macros ********/
// Status
#define LOW                0
#define HIGH               1

// DigitalWrite
#define DIGITAL_WRITE(...)                      DIGITAL_WRITE_SUB(__VA_ARGS__)
#define DIGITAL_WRITE_SUB(PORT, PIN, STATUS)    { (STATUS==HIGH) ? (P##PORT##OUT |= (1 << PIN)) : (P##PORT##OUT &= ~(1 << PIN)); }

// PinToggle
#define PIN_TOGGLE(...)                         PIN_TOGGLE_SUB(__VA_ARGS__)
#define PIN_TOGGLE_SUB(PORT,PIN)                { P##PORT##OUT ^=  (1<<PIN); }

/******** PinRead Macros ********/
#define DIGITAL_READ(...)                       DIGITAL_READ_SUB(__VA_ARGS__)
#define DIGITAL_READ_SUB(PORT, PIN)             ((P##PORT##IN>>PIN) & (0x1))

#endif // _GPIO_OW_H_
