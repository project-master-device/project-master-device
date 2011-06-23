/*
 * Copyright (c) 2010 MikloBit
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
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
 *
 * @(#)$
 */

#ifndef _CAN_H_
#define _CAN_H_

#include "contiki.h"
#include "can-drv.h"

/**
 * Event posted when a can command is completed or aborted.
 *
 * A data pointer to the command structure
 * is sent together with the event.
 */
extern process_event_t can_event_message;


PROCESS_NAME(can_process);

// ----------
// @brief This constant is used as return value for "can_cmd" function.
#define CAN_CMD_REFUSED  0xFF
// ----------
// @brief This constant is used as return value for "can_cmd" function.
#define CAN_CMD_ACCEPTED         0x00

// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_COMPLETED     0x00
// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_NOT_COMPLETED 0x01
// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_ERROR         0x02

// ----------
// @brief This enumeration is used to select an action for a specific message
// declared in can_st_cmd_t structure.
typedef enum {
  CAN_CMD_NONE,
  CAN_CMD_TX,
  CAN_CMD_TX_DATA,
  CAN_CMD_TX_REMOTE,
  CAN_CMD_RX,
  CAN_CMD_RX_DATA,
  CAN_CMD_RX_REMOTE,
  CAN_CMD_RX_MASKED,
  CAN_CMD_RX_DATA_MASKED,
  CAN_CMD_RX_REMOTE_MASKED,
  CAN_CMD_REPLY,
  CAN_CMD_REPLY_MASKED,
  CAN_CMD_ABORT
} can_cmd_t;

// ----------
// @brief This union defines a CAN identifier and allows to access it in mode
// standart, extended or throught a table.
typedef union{
  uint32_t ext;
  uint16_t std;
  uint8_t  tab[4];
} can_id_t;

// ----------
// @brief This structure defines some specific information as RTR bit and
// IDE bit
typedef struct{
  Bool rtr;
  Bool ide;
} can_ctrl_t;

// ----------
// @brief This structure allows to define a specific action on CAN network.
// 0) next  :  pointer to next waiting command in list
// 1) handle: manage by the library.
// 2) cmd   :  initialize by the application to select the operation.
// 3) id    :  initialize by the application in transmission
//             complete by the library in reception.
// 4) dlc   :  initialize by the application to give the number of data to
//             transmit complete by the library in reception.
// 5) pt_data: pointer on the table which contains the data to send or
//             received.
// 6) status:  manage by the library.
// 7) ctrl  :  field ide to signal a extended frame .
// 8) timestamp_lo : lower part of message timestamp - is get from MOB
// 9) timestamp_hi : higher part of message timestamp - is get from variable incremented by OVRIT interrupt
// 10) p		:  pointer to process sending command. If not null, command complete will trigger event to sender process


typedef  struct{
  void 	*next ;
  uint8_t         handle;
  can_cmd_t  cmd;
  can_id_t   id;
  uint32_t   msk;
  uint8_t         dlc;
  uint8_t*        pt_data;
  uint8_t         status;
  can_ctrl_t ctrl;
  uint16_t timestamp_lo ;
  uint16_t timestamp_hi ;
  struct process *p;
} can_st_cmd_t;


uint8_t can_init(uint8_t mode);
void 	can_cmd(can_st_cmd_t* cmd);
uint8_t can_get_status (can_st_cmd_t *);

#endif // _CAN_H_
