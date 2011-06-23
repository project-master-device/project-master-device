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
 *       Process wait for incoming CAN frame .
 *       constant CAN_BAUDRATE has to be defined in contiki-conf.h
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */

#include <string.h>
#include <stdio.h>
#include "cantest-rx.h"
#include "dev/can.h"
#include "remote-control.h"

PROCESS(cantest_rx_process, "CAN test RX");

#define CAN_ID 0x80       //- 128 decimal

/*-----------------------------------------------------------------------------------*/
PROCESS_THREAD(cantest_rx_process, ev, data)
{
  static uint8_t rx_buffer[9];
  static can_st_cmd_t rx_msg;
  static uint8_t message_rx[] = "UCAN wait for frame\n";
  static uint8_t control_message[100];
  static uint8_t response_message[50] = "UCAN received frame time stamp = \n";
  static uint8_t unknown_event[] = "UUnknown event\n";
  static can_st_cmd_t *can_msg;

  static uint8_t i;

  PROCESS_BEGIN();

  rx_msg.pt_data = &rx_buffer[0];
  rx_msg.status = 0;

//  if( REMOTE_CONNECTED() )
//	  process_post(&remote_control_process, remote_control_event, control_message1 );

  while(1) {
//	    etimer_set(&etimer, CLOCK_SECOND / 2 );

        // --- Init Rx Commands
  	    for( i=0; i<8; i++) {
  	    	rx_buffer[i]=0;
  	    }
        rx_msg.id.std = CAN_ID;
        rx_msg.ctrl.ide = 0;
        rx_msg.ctrl.rtr = 0;
        rx_msg.dlc = 8;
        rx_msg.p = PROCESS_CURRENT();    // command will trigger event when complete
        rx_msg.cmd = CAN_CMD_RX_DATA_MASKED;
        // --- Rx Command
        can_cmd(&rx_msg);

	    PROCESS_WAIT_EVENT();

        if( REMOTE_CONNECTED() )
        {
			can_msg = &rx_msg ;
			rx_buffer[8] = 0;
			sprintf(control_message,"URx stamp=%04x-%04x data=%s\n",
					                 can_msg->timestamp_hi,
					                 can_msg->timestamp_lo,
					                 rx_buffer );


	       process_post(&remote_control_process, remote_control_event, control_message );
        }


  }
  PROCESS_END();
}






