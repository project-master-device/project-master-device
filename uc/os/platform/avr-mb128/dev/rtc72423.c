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

#include "rtc72423.h"


uint8_t *rtc = (uint8_t *)RTC_BASE ;  // external memory space address


/* set date and time */

void rtc72423_set_clock( struct rtc72423_date *d, struct rtc72423_time *t )
{
  rtc[15]  = 7 ;  /* init clock */
  if( d )
  {
      rtc[12] = d->week ;
      rtc[11] = d->year10 ;
      rtc[10] = d->year ;
      rtc[9] = d->month10 ;
      rtc[8] = d->month ;
      rtc[7] = d->day10 ;
      rtc[6] = d->day ;
  }
  if( t )
  {
      rtc[5] = t->hour10 ;
      rtc[4] = t->hour ;
      rtc[3] = t->min10 ;
      rtc[2] = t->min ;
      rtc[1] = t->sec10 ;
      rtc[0] = t->sec ;
  };
  rtc[13] = 0 ; /* start clock */
  rtc[15] = 4 ;
}

/* read time HH:MM:SS */

struct rtc72423_time *rtc72423_get_time( struct rtc72423_time *t )
{
 t->hour10 = rtc[5] & 0x0f ;
 t->hour = rtc[4] & 0x0f ;
 t->min10 = rtc[3] & 0x0f ;
 t->min = rtc[2] & 0x0f ;
 t->sec10 = rtc[1] & 0x0f ;
 t->sec = rtc[0] & 0x0f ;
 return( t ) ;
}

/* odczytanie daty YY:MM:DD */

struct rtc72423_date *rtc72423_get_date( struct rtc72423_date *d )
{
 d->week =  rtc[12] & 0x0f ;
 d->year10 = rtc[11] & 0x0f ;
 d->year = rtc[10] & 0x0f ;
 d->month10 = rtc[9] & 0x0f ;
 d->month = rtc[8] & 0x0f ;
 d->day10 = rtc[7] & 0x0f ;
 d->day = rtc[6] & 0x0f ;
 return( d );
}


/* convert time HH:MM:SS  to string */

uint8_t *rtc72423_time2str(struct rtc72423_time *t ,  uint8_t *str )
{
  str[0] = ( t->hour10 + '0');
  str[1] = ( t->hour + '0');
  str[2] = (':');
  str[3] = ( t->min10 + '0');
  str[4] = ( t->min + '0');
  str[5] = (':');
  str[6] = (t->sec10 + '0');
  str[7] = (t->sec + '0');
  str[8] = 0 ;
  return str;
}

/* get time as formatted string */

uint8_t *rtc72423_get_time_str(uint8_t *str )
{
  static struct rtc72423_time t;

  return rtc72423_time2str(rtc72423_get_time(&t),str);
}


/* convert date YY:MM:DD  to string  */

uint8_t *rtc72423_date2str(struct rtc72423_date *d ,  uint8_t *str )
{

  str[0] = ( d->year10 + '0');
  str[1] = ( d->year + '0');
  str[2] = ('/');
  str[3] = ( d->month10 + '0');
  str[4] = ( d->month + '0');
  str[5] = ('/');
  str[6] = (d->day10 + '0');
  str[7] = (d->day + '0');
  str[8] = 0 ;
  return str;
}

/* get date as formatted string */

uint8_t *rtc72423_get_date_str(uint8_t *str )
{
  static struct rtc72423_date d;

  return rtc72423_date2str(rtc72423_get_date(&d),str);
}
