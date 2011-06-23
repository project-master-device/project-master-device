#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include <stdint.h>

////Device types
//enum device_type {
//    DEVICE_LED,
//    DEVICE_BUTTON,
//    DEVICE_READER
//};
////Modules -- one entry for each module
////MODULE_NUMBER should be last (to indicate number of modules)
//enum modules {
//    MODULE_LED1, MODULE_NUMBER
//};

// Clock ticks per second
#define CLOCK_CONF_SECOND 125
// Time quantum for ftimer
#define FTIMER_Q 1

#define AVR_CONF_USE32KCRYSTAL
#define MCU_MHZ 16

#define CCIF
#define CLIF

#define PLATFORM PLATFORM_AVR

#define CAN_RECV_BUF_MAX 8

//#define LED1_DDR DDRB
//#define LED1_PORT PORTB
//#define LED1_OFFSET PB4

#define BUTTON1_DDR DDRB
#define BUTTON1_PIN PINB
#define BUTTON1_OFFSET PB7

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

#endif /*__CONTIKI_CONF_H__ */
