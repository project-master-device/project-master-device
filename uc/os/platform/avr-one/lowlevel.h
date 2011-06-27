#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_

#include <avr/io.h>

void lowlevel_init();

void blink(volatile uint8_t * port, uint8_t shift, unsigned int n, unsigned int interval_ms);

void led1_on();
void led1_off();
void led1_toggle();
void led1_blink(unsigned int n, unsigned int interval_ms);

void led2_on();
void led2_off();
void led2_toggle();
void led2_blink(unsigned int n, unsigned int interval_ms);

void led3_on();
void led3_off();
void led3_toggle();
void led3_blink(unsigned int n, unsigned int interval_ms);

uint8_t button_state();

#define REV3_BOARD
//#define REV1_BOARD

// Periphery
#if defined REV3_BOARD
#define BUTTON_DDR  DDRB
#define BUTTON_PIN  PINB
#define BUTTON      PB7
#define BUTTON_ON   0
#define BUTTON_OFF  1
#elif defined REV1_BOARD
#define BUTTON_DDR  DDRC
#define BUTTON_PIN  PINC
#define BUTTON      PC0
#define BUTTON_ON   1
#define BUTTON_OFF  0
#endif

#if defined REV3_BOARD
#define LED1_DDR    DDRB
#define LED2_DDR    DDRB
#define LED3_DDR    DDRB
#elif defined REV1_BOARD
#define LED1_DDR    DDRC
#define LED2_DDR    DDRC
#define LED3_DDR    DDRC
#endif

#if defined REV3_BOARD
#define LED1_PORT   PORTB
#define LED2_PORT   PORTB
#define LED3_PORT   PORTB
#elif defined REV1_BOARD
#define LED1_PORT   PORTC
#define LED2_PORT   PORTC
#define LED3_PORT   PORTC
#endif


#if defined REV3_BOARD
#define LED1        PB4
#define LED2        PB5
#define LED3        PB6
#elif defined REV1_BOARD
#define LED1        PC1
#define LED2        PC2
#define LED3        PC1
#endif

#endif /* LOWLEVEL_H_ */
