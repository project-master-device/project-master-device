#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include <stdint.h>

enum device_type {
    DEVICE_LED,
    DEVICE_BUTTON
};

// Clock ticks per second
#define CLOCK_CONF_SECOND 125
#define AVR_CONF_USE32KCRYSTAL

#define MCU_MHZ 16

#define CCIF
#define CLIF

#define PLATFORM PLATFORM_AVR

#define LED1PIN 0

typedef unsigned short clock_time_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned long u32_t;
typedef unsigned short uip_stats_t;
typedef unsigned long off_t;
typedef int32_t s32_t;

#define RS232_PORT_1 1

// COM port to be used for SLIP connection
#define SLIP_PORT RS232_PORT_1

#define ETIMSK TIMSK3
#define ETIFR TIFR3
#define TICIE3 ICIE3

#define CONFIG_OPTION_KEY_LEN   16
#define CONFIG_SECTION_NAME_LEN 16

#define CAN_BAUDRATE_VARIABLE

typedef unsigned int size_t;

// Periphery
#define BUTTON_DDR  DDRB
#define BUTTON_PIN  PINB
#define BUTTON      PB7

#define LED1_DDR    DDRB
//#define LED1_DDR    DDRE
#define LED2_DDR    DDRB
#define LED3_DDR    DDRB
//#define LED2_DDR    DDRC
//#define LED3_DDR    DDRC

#define LED1_PORT   PORTB
//#define LED1_PORT   PORTE
#define LED2_PORT   PORTB
#define LED3_PORT   PORTB
//#define LED2_PORT   PORTC
//#define LED3_PORT   PORTC

#define LED1        PB4
//#define LED1        PE7
#define LED2        PB5
#define LED3        PB6
//#define LED2        PC1
//#define LED3        PC0

#endif /*__CONTIKI_CONF_H__ */
