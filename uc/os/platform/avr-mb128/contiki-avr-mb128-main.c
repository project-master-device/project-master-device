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
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
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
 * @(#)$$
 */

/**
 * \file
 *         Sample Contiki kernel for MB-128/MB-128-USBCAN development boards
 *
 * \author
 *         Milosz Klosowicz <support@miklobit.com>
 */

#include <avr/pgmspace.h>
#include <stdio.h>

#include "contiki-avr-mb128.h"
#include "cfs/cfs.h"
#include "lib/mmem.h"
#include "loader/symbols-def.h"
#include "loader/symtab.h"
#include "sys/mt.h"

#ifdef TESTPROCESS
#include "test-process.h"
#endif

#ifdef REMOTE_CONTROL
#include "remote-control.h"
#endif

#ifdef WEBSERVER
#include "webserver-nogui.h"
#endif

#ifdef CAN_TEST_TX
#include "cantest-tx.h"
#endif
#ifdef CAN_TEST_RX
#include "cantest-rx.h"
#endif

#ifdef CANCONVERTER
#include "canconverter.h"
#endif

#ifdef CONTIKI_WITH_NET
   PROCINIT(&etimer_process, &tcpip_process, &uip_fw_process, &can_process );
#else
   PROCINIT(&etimer_process, &can_process );
#endif

int main(void)
{
  /*
   * GCC depends on register r1 set to 0.
   */
  asm volatile ("clr r1");

  /* Initialize hardware */
  init_lowlevel();

  leds_on( LEDS_LED1 );
  /* Clock */
  clock_init();

  /* Process subsystem */
  process_init();
  /* Register initial processes */
  procinit_init();

#ifdef CONTIKI_WITH_NET
  /* Network support (uIP) */
  init_net();
#endif

  /* Initalize heap allocator */
  mmem_init ();

  // autostart_start(autostart_processes);

#ifdef  TESTPROCESS
  process_start(&test_process1, NULL);
#endif

#ifdef WEBSERVER
  process_start(&webserver_nogui_process, NULL);
#endif

#ifdef REMOTE_CONTROL
  process_start(&remote_control_process, NULL);
#endif

#ifdef CAN_TEST_TX
    process_start(&cantest_tx_process, NULL);
#endif
#ifdef CAN_TEST_RX
    process_start(&cantest_rx_process, NULL);
#endif

#ifdef CANCONVERTER
    process_start(&can_converter_process, NULL);
#endif

  rs232_print_p (DEBUG_PORT, PSTR ("System Initialized.\n"));

  /* Main scheduler loop */
  while(1) {
    process_run();
  }

  return 0;
}

