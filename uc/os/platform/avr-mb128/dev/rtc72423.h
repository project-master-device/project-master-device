/*
 * Copyright (c) 2010, MikloBit
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
 * Author: Milosz Klosowicz <support@miklobit.com>
 *
 * @(#)$
 */
#ifndef RTC72423_H_
#define RTC72423_H_

#include "contiki-conf.h"

#ifndef RTC_BASE
#define RTC_BASE 0x6000  // clock address base with default GAL config
#endif

struct rtc72423_date
{
    uint8_t year10 ;
    uint8_t year ;
    uint8_t month10 ;
    uint8_t month ;
    uint8_t day10 ;
    uint8_t day ;
    uint8_t week ;
};

struct rtc72423_time
{
    uint8_t hour10 ;
    uint8_t hour ;
    uint8_t min10 ;
    uint8_t min ;
    uint8_t sec10 ;
    uint8_t sec ;
};

/* set date and time */
void rtc72423_set_clock( struct rtc72423_date *d, struct rtc72423_time *t );
/* read time HH:MM:SS */
struct rtc72423_time *rtc72423_get_time( struct rtc72423_time *t );
/* read date YY:MM:DD */
struct rtc72423_date *rtc72423_get_date( struct rtc72423_date *d );
/* convert time HH:MM:SS  to string */
uint8_t *rtc72423_time2str(struct rtc72423_time *t ,  uint8_t *str );
/* convert date YY:MM:DD  to string  */
uint8_t *rtc72423_date2str(struct rtc72423_date *d ,  uint8_t *str );
/* get time as formatted string */
uint8_t *rtc72423_get_time_str(uint8_t *str ) ;
/* get date as formatted string */
uint8_t *rtc72423_get_date_str(uint8_t *str ) ;

#endif /* RTC72423_H_ */
