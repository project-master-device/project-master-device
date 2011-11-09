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
 *       I/O control process
 *       I/O port input values are sent every 1 second to remote_control_process
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */


#include "io-control.h"
#include "remote-control.h"




PROCESS(io_control_process, "IO control");
PROCESS_THREAD(io_control_process, ev, data)
{
  static struct etimer etimer;
  static uint8_t ioBuf[30];
  static uint8_t ioValue ;
  static uint8_t i;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&etimer, CLOCK_SECOND  );
    PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
    if( REMOTE_CONNECTED() )
    {
    	// send IO message
    	sprintf( ioBuf,"IBI%02x\nIDI%02x\nIEI%02x\n", PINB, PIND, PINE );
    	process_post(&remote_control_process, remote_control_event, ioBuf );
    }
  }

  PROCESS_END();
}

static uint8_t htoi( uint8_t val ) {
	if( val >= '0' && val <= '9' ) {
		return (val - '0');
	}
	else {
	    return ( toupper( val ) - 'A' + 10 );
	}
}

static uint8_t get_hex_value( uint8_t *ptr ) {
   return (htoi(ptr[0]) << 4 ) | htoi(ptr[1]) ;
}


void io_control_set_io( uint8_t *ptr ) {
	uint8_t val = get_hex_value(&ptr[2]) ;
	switch( ptr[0] ) {
		case 'B':
			switch( ptr[1] ) {
			  case 'O':  PORTB = val ; break ;
			  case 'D':  DDRB = val ; break ;
			}; break ;
		case 'D':
			switch( ptr[1] ) {
			  case 'O':  PORTD = val ; break ;
			  case 'D':  DDRD = val ; break ;
			}; break ;
		case 'E':
			switch( ptr[1] ) {
			  case 'O':  PORTE = val ; break ;
			  case 'D':  DDRE = val ; break ;
			}; break ;
	}

}
