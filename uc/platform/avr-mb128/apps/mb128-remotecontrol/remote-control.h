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
 *       Header file for remote control process
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */


#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__


#include <stdio.h>
//#include "dev/rs232.h"
//#include "dev/leds.h"
//#include "dev/adc.h"
//#include "dev/rtc72423.h"
#include "contiki.h"
#include "contiki-net.h"

extern process_event_t remote_control_event;

PROCESS_NAME(remote_control_process);

#ifndef REMOTE_CONTROL_BUFSIZE
#define REMOTE_CONTROL_BUFSIZE 1000
#endif

struct remote_control_state_t {
  char bufin[REMOTE_CONTROL_BUFSIZE + 1];
  char bufout[REMOTE_CONTROL_BUFSIZE + 1];
  char lenout;
  uint16_t numsent;
  u8_t state;
#define STATE_CONNECTED 0
#define STATE_CLOSED  	1
};

extern struct remote_control_state_t remote_control_state ;

#define REMOTE_CONNECTED()  ( remote_control_state.state == STATE_CONNECTED )

#endif /* #ifndef __REMOTE_CONTROL_H__ */
