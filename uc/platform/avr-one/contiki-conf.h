/*
 * Copyright (c) 2010-2011 Sergey Gnuskov, Timofey Kirillov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


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
