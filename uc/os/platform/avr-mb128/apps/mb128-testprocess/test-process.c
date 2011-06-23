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

/**
 * \file
 *       Sample loadable module
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */


#include "test-process.h"

PROCESS(test_process1, "Test process");
PROCESS_THREAD(test_process1, ev, data)
{
  static struct etimer etimer;
  static uint8_t convBuf[20];
  static uint8_t adcValue ;
  static struct rtc72423_date d = { 1,0,  0,5,  0,8  };
  static struct rtc72423_time t = { 1,2,  0,0,  0,0  };

  PROCESS_BEGIN();

  rtc72423_set_clock( &d, &t );
  rs232_print_p (DEBUG_PORT, PSTR("test_process1 starting\n"));

  while(1) {
    etimer_set(&etimer, CLOCK_SECOND);
    PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
    leds_on( LEDS_LED2 );
//    process_post(&remote_control_process, remote_control_event, get_time_str(convBuf));

//    adcValue = adc_convert_8bit(ADC_CH_ADC0);
//    sprintf(convBuf,"\nADC(0) = %d", adcValue);
//    rs232_print (DEBUG_PORT, convBuf );
//    rs232_print (DEBUG_PORT, "\nTime : ");
//    rs232_print (DEBUG_PORT, get_time_str(convBuf) );

    etimer_set(&etimer, CLOCK_SECOND);
    PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
    leds_off( LEDS_LED2 );
//    process_post(&remote_control_process, remote_control_event, get_time_str(convBuf));


//    adcValue = adc_convert_8bit(ADC_CH_ADC1);
//    sprintf(convBuf,"\nADC(1) = %d", adcValue);
//    rs232_print (DEBUG_PORT, convBuf );
//    rs232_print (DEBUG_PORT, "\nDate : ");
//    rs232_print (DEBUG_PORT, get_date_str(convBuf) );
  }

  PROCESS_END();
}

// AUTOSTART_PROCESSES(&test_process1);
