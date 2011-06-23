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
 * 3  Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE copyright holders AND CONTRIBUTORS ``AS IS'' AND
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
 *       Module to remote control MB-128 board from Java applet.
 *       Module wait for incoming socket connection on port 81
 *       Command string format:
 *       "TYY/MM/DDHH:MM:SS\n"
 *       	- applet -> module : set rtc date/time
 *			- module -> applet : send current time ( every 1 s )
 *		 "U uart data until new line\n"
 *		 	- applet -> module : send one data line  via uart out of module
 *		  	- module -> applet : display one data line received from uart
 *		 "Axx0xx1xx2xx3xx4xx5xx6xx7xx8\n"
 *		 	- module -> applet : send ADC values ( channel 0 first ), format hex 3 digit
 *		 "IPTxx\n"
 *		 	- P - port num: B,D,E
 *		    - T - port register: I - PIN, O - PORT, D - DDR
 *		    - xx - register value : 2 digit hex
 *		 	- module -> applet : get I/O port input lines ( PORTB, PORTD, PORTE ), format hex 2 digit
 *		 	- applet -> module : set I/O port output lines( PORTB, PORTC, PORTD )
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */

#include <string.h>
#include <stdio.h>
#include "remote-control.h"
#include "dev/leds.h"
#include "time-control.h"
#include "adc-control.h"
#include "io-control.h"


PROCESS(remote_control_process, "Remote control");

process_event_t remote_control_event;

// AUTOSTART_PROCESSES(&remote_control_process);

#define MIN(a, b) ((a) < (b)? (a): (b))

#define ISO_nl       0x0a
#define ISO_cr       0x0d


struct remote_control_state_t remote_control_state;
static struct uip_conn *conn;
static struct psock ps;

static void remote_control_connected( void )
{
	conn = uip_conn;
	tcp_markconn(conn, &remote_control_state);
    remote_control_state.lenout = 0;
    remote_control_state.state = STATE_CONNECTED;
    PSOCK_INIT(&ps, remote_control_state.bufin, sizeof(remote_control_state.bufin) - 1);
    leds_on( LEDS_LED3 );    // led on  when conected
    process_start(&time_control_process, NULL);
    process_start(&adc_control_process, NULL);
    process_start(&io_control_process, NULL);

}

/*---------------------------------------------------------------------------*/
static void remote_control_closed( void )
{
    remote_control_state.state = STATE_CLOSED;
    leds_off( LEDS_LED3 );   // led off  when disconnected
    process_exit(&time_control_process);
    process_exit(&adc_control_process);
    process_exit(&io_control_process);

}

/*-----------------------------------------------------------------------------------*/
/** \internal
 * The uIP event handler.
 * Dispatch received message
 */
/*-----------------------------------------------------------------------------------*/

static PT_THREAD(remote_control_appcall())
{
	PSOCK_BEGIN(&ps);

	if( uip_poll() && ( remote_control_state.lenout > 0 )) {
		PSOCK_SEND(&ps, remote_control_state.bufout, remote_control_state.lenout );
		remote_control_state.lenout = 0;
	}
	if( uip_newdata())  {
		PSOCK_READTO(&ps, '\n');
		if( PSOCK_DATALEN(&ps) )  {
			// dispatch message type
			switch( remote_control_state.bufin[0] ) {
				case 'U':  rs232_print (DEBUG_PORT, &(remote_control_state.bufin[1]) ); rs232_print_p (DEBUG_PORT, PSTR( "\n" )); break ;
				case 'C':  /* send data to CAN bus */ break ;
				case 'A':  /* request to ADC monitor ( set time period ) */ break ;
				case 'I':  io_control_set_io( &(remote_control_state.bufin[1]) ); break ;
				case 'T':  time_control_set_time( &(remote_control_state.bufin[1])) ;  break ;
				default:  PSOCK_SEND_STR(&ps, "UWrong message type\n");   break ;
							// PSOCK_SEND(&ps, s.bufin, PSOCK_DATALEN(&ps));

			}
		}
	}


	PSOCK_END(&ps);
}

/*-----------------------------------------------------------------------------------*/
PROCESS_THREAD(remote_control_process, ev, data)
{
//  static struct etimer etimer;
//	static void *send_data ;
  static uint8_t len;

  PROCESS_BEGIN();

  remote_control_event = process_alloc_event();
  remote_control_closed();

  tcp_listen(HTONS(81));

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_connected()) {
    	remote_control_connected();
    	while(!(uip_aborted() || uip_closed() || uip_timedout())) {
    		PROCESS_WAIT_EVENT();
    		if(ev == tcpip_event) {
    			remote_control_appcall();
    		}
    		else if (ev == remote_control_event) {
    			len = strlen( data );
    			if( ( remote_control_state.lenout + len) <  REMOTE_CONTROL_BUFSIZE ) {
    				strcpy( &remote_control_state.bufout[remote_control_state.lenout], data );
    				remote_control_state.lenout += len ;
    				tcpip_poll_tcp(conn);
    			}
    		}
    	}
    	remote_control_closed();
    }
  }

  PROCESS_END();
}






