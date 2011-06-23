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
 *       ADC control process
 *       ADC values are sent every 1 second to remote_control_process
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */


#include "adc-control.h"
#include "remote-control.h"




PROCESS(adc_control_process, "Adc control");
PROCESS_THREAD(adc_control_process, ev, data)
{
  static struct etimer etimer;
  static uint8_t adcBuf[30];
  static uint16_t adcValue ;
  static uint8_t i;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&etimer, CLOCK_SECOND  );
    PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
    if( REMOTE_CONNECTED() )
    {
    	// send ADC message
    	adcBuf[0] = 'A';
    	for( i = 0; i < 8; i++ ) {
    		adcValue = adc_convert_10bit( i );
    		sprintf(&adcBuf[i*3+1],"%03x", adcValue);
    	}
    	adcBuf[25] = '\n';
    	adcBuf[26] = 0;
    	process_post(&remote_control_process, remote_control_event, adcBuf );
    }
  }

  PROCESS_END();
}

