/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov, Timofey Kirillov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#ifndef CAN_DRV_H_
#define CAN_DRV_H_

#include "can_drv_structs.h"
//#include "list.h"

#define CAN_DRV_RC_NORM					0
#define CAN_DRV_RC_TIMEOUT				1
#define CAN_DRV_RC_TIMER_ERR			2
#define CAN_DRV_RC_WRONG_DATA			3

typedef void (*can_send_callback_t)(const int, can_frame_t *, void *);
typedef void (*can_recv_callback_t)(can_frame_t *);

typedef uint32_t can_mask_t;

int can_init(can_speed_t can_speed, can_send_callback_t send_callback_, can_recv_callback_t recv_callback_,
		uint32_t send_timeout_real_us , can_mask_t masked_id, can_mask_t mask);
int can_send(can_frame_t * can_frame, void * context);

void can_set_mask(can_mask_t masked_id, can_mask_t mask);
int can_set_speed(can_speed_t can_speed);

#ifdef CONTIKI
#include "process.h"

PROCESS_NAME(can_drv_process);

#endif

#endif /* CAN_DRV_H_ */
