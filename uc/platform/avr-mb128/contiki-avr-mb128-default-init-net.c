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
 * Author: Simon Barner <barner@in.tum.de>
 *
 * @(#)$$
 */

#include "contiki-avr-mb128.h"



static struct uip_fw_netif slipif_200 =
  {UIP_FW_NETIF(10,200,0,0, 255,255,0,0, slip_send)};
static struct uip_fw_netif slipif_100 =
    {UIP_FW_NETIF(10,100,0,0, 255,255,0,0, slip_send)};


void
init_net(void)
{
	// if PE.7 pin = 0 adress = 10,100,0,2
	// if PE.7 pin = 1 adress = 10,200,0,2

  uip_ipaddr_t hostaddr;

  rs232_set_input (SLIP_PORT, slip_input_byte);
  process_start(&slip_process, NULL);

  DDRE &= 0x7F ;  // PE.7 = input
  PORTE |=0x80 ;  // PE.7 pull-up on
  if( PINE & 0x80 )
  {
	  uip_ipaddr(&hostaddr, 10, 200, 0, 2);
	  uip_sethostaddr(&hostaddr);
	  uip_fw_register(&slipif_200);
  }
  else
  {
	  uip_ipaddr(&hostaddr, 10, 100, 0, 2);
	  uip_sethostaddr(&hostaddr);
	  uip_fw_register(&slipif_100);
  }

}
