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
 *       process send test CAN frame every second.
 *       constant CAN_BAUDRATE has to be defined in contiki-conf.h
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */

#include <string.h>
#include <stdio.h>
#include "cantest-tx.h"
#include "dev/can.h"
#include "remote-control.h"

PROCESS(cantest_tx_process, "CAN test TX");

#define CAN_MY_ID 0x80       //- 128 decimal

/*-----------------------------------------------------------------------------------*/
PROCESS_THREAD(cantest_tx_process, ev, data)
{
  static struct etimer etimer;

  static uint8_t can_buffer[20];  // only first 8 bytes are used by CAN message
  static can_st_cmd_t tx_message;
  // messages to display in UART window of remote control applet
  static uint8_t control_message[] = "UCAN TX frame sended\n";
  static uint32_t counter ;

  // --- Init response data
  tx_message.pt_data = &can_buffer[0];

  PROCESS_BEGIN();

  counter = 0;

  while(1) {
	  sprintf(can_buffer,"%08x",counter++);
	  tx_message.ctrl.ide = 0;            //- CAN 2.0A
	  tx_message.dlc = 8;                 //- Message with 8-byte data
	  tx_message.id.std = CAN_MY_ID;
	  tx_message.p = NULL ;   			  // command complete  don't send event to us
	  tx_message.cmd = CAN_CMD_TX_DATA;

      can_cmd(&tx_message);
      if( REMOTE_CONNECTED() )
    	  process_post(&remote_control_process, remote_control_event, control_message );

	  etimer_set(&etimer, CLOCK_SECOND );
      PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);   // wait 1 s

      // in case CAN frame could not be sent
      tx_message.cmd = CAN_CMD_ABORT;
  	  can_cmd(&tx_message);


  }
  PROCESS_END();
}






