/*
 * Copyright (c) 2006, Technical University of Munich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * @(#)$$
 */

/**
 * \file
 *         Configuration for sample STK 501 Contiki kernel
 *
 * \author
 *         Simon Barner <barner@in.tum.de
 */

#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include <stdint.h>

typedef int32_t s32_t;

/*
 * MCU and clock rate
 */

#ifndef MCU_MHZ
#if defined (__AVR_ATmega128__)
#define MCU_MHZ 11    /*  exact. 11.0592 Mhz */
#elif defined(__AVR_AT90CAN128__)
#define MCU_MHZ 16
#define CAN_BAUDRATE 100  /* 100Kb/s */
#endif
#endif

#define PLATFORM PLATFORM_AVR

/* Clock ticks per second */
#define CLOCK_CONF_SECOND 125

/* COM port to be used for SLIP connection */
#define SLIP_PORT RS232_PORT_1
/* COM port to be used for CAN/RS232 interface */
#define CAN_PORT SLIP_PORT
/* COM port to be used for DEBUG information */
#define DEBUG_PORT RS232_PORT_0

#define RS232_CONF_PRINTF_BUFFER_LENGTH 128

/* Pre-allocated memory for loadable modules heap space (in bytes)*/
#define MMEM_CONF_SIZE 256

/* Use the following address for code received via the codeprop
 * facility
 */
#define EEPROMFS_ADDR_CODEPROP 0x8000

/* LED masks */
#define LEDS_LED1  ( 1 << 4 )
#define LEDS_LED2  ( 1 << 5 )
#define LEDS_LED3  ( 1 << 6 )
#define LEDS_LED4  ( 1 << 7 )
/** LED mask for all the LEDs on the board. */
#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2 | LEDS_LED3 | LEDS_LED4)
/** LED mask for the none of the board LEDs */
#define LEDS_NO_LEDS          0

#define CCIF
#define CLIF

#ifdef CONTIKI_WITH_NET
#define UIP_CONF_PINGADDRCONF    0
#define UIP_CONF_MAX_CONNECTIONS 5
#define UIP_CONF_MAX_LISTENPORTS 10
#define UIP_CONF_BUFFER_SIZE     1024  // using max default buffer size
#define UIP_CONF_TCP_SPLIT       1
#define UIP_CONF_FWCACHE_SIZE    2
#define UIP_CONF_BROADCAST       1
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1

#define UIP_CONF_IPV6            0
#if UIP_CONF_IPV6
#define UIP_CONF_IPV6_CHECKS     1
#define UIP_CONF_IPV6_QUEUE_PKT  1
#define UIP_CONF_IPV6_REASSEMBLY 0
#define UIP_CONF_ICMP6           1
#endif
#define UIP_CONF_NETIF_MAX_ADDRESSES  3
#define UIP_CONF_ND6_MAX_PREFIXES     3
#define UIP_CONF_ND6_MAX_NEIGHBORS    4
#define UIP_CONF_ND6_MAX_DEFROUTERS   2
#endif

typedef unsigned char  Bool;
typedef unsigned short clock_time_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned long u32_t;
typedef unsigned short uip_stats_t;
typedef unsigned long off_t;

void clock_delay(unsigned int us2);
void clock_wait(int ms10);
void clock_set_seconds(unsigned long s);
unsigned long clock_seconds(void);

#endif /* __CONTIKI_CONF_H__ */
