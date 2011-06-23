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
 *       Time control process
 *       Date/time values are sent every 1 second to remote_control_process
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */


#include "time-control.h"
#include "remote-control.h"




PROCESS(time_control_process, "Time control");
PROCESS_THREAD(time_control_process, ev, data)
{
  static struct etimer etimer;
  static uint8_t timeBuf[30];

  PROCESS_BEGIN();

  while(1) {
	  etimer_set(&etimer, CLOCK_SECOND  );
	  PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
	  if( REMOTE_CONNECTED() )
	  {
		  // send date/time message
		  timeBuf[0] = 'T';
		  rtc72423_get_date_str(timeBuf+1);
		  rtc72423_get_time_str(timeBuf+9);
		  timeBuf[17] = '\n';
		  timeBuf[18] = 0;
		  process_post(&remote_control_process, remote_control_event, timeBuf );
	  }
  }

  PROCESS_END();
}

void time_control_set_time( uint8_t *ptr ){
	static struct rtc72423_date d ;
	static struct rtc72423_time t ;

	d.year10 = ptr[0] - '0';
	d.year = ptr[1] - '0';
	d.month10 = ptr[3] - '0';
	d.month = ptr[4] - '0';
	d.day10 = ptr[6] - '0';
	d.day = ptr[7] - '0';
	t.hour10 = ptr[8] - '0';
	t.hour = ptr[9] - '0';
	t.min10 = ptr[11] - '0';
	t.min = ptr[12] - '0';
	t.sec10 = ptr[14] - '0';
	t.sec = ptr[15] - '0';
	rtc72423_set_clock( &d, &t );
}

