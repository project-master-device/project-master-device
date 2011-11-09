#ifndef CONTIKI_CONF_H_
#define CONTIKI_CONF_H_

#include <stdint.h>

#define WIEGAND_BITS 26

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

#endif /* CONTIKI_CONF_H */
