#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include <avr/io.h>

// Periphery
#define BUTTON_DDR  DDRB
#define BUTTON_PIN  PINB
#define BUTTON      PB7

#define LED1_DDR    DDRB
//#define LED1_DDR    DDRE
#define LED2_DDR    DDRB
#define LED3_DDR    DDRB

#define LED1_PORT   PORTB
//#define LED1_PORT   PORTE
#define LED2_PORT   PORTB
#define LED3_PORT   PORTB

#define LED1        PB4
//#define LED1        PE7
#define LED2        PB5
#define LED3        PB6

#endif /*__CONTIKI_CONF_H__ */
