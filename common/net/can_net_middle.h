/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov
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


#ifndef CAN_NET_MIDDLE_H_
#define CAN_NET_MIDDLE_H_

//TODO: rename to can_net_core

#include "can_net_core.h"
#include "../lib/bytearray.h"

/* defines:
		CAN_NET_CONFIRMATION	- use confirmations on msg
*/
#define CAN_NET_CONFIRMATION

/* -------------------------------------INTERFACE_STRUCTS---------------------------------------*/

//TODO:CANNET_REFACTORING make file can_net_interface_structs

/**
 * @struct msg_metadata_t
 * @brief header for message
 */
typedef struct {
	uint32_t hw_addr;
	uint8_t port;
	uint8_t is_system;
	uint16_t id;
} msg_metadata_t;

typedef struct {
	msg_metadata_t meta;
	bytearr_t data;
} msg_lvl2_t;

typedef struct {
	uint8_t port_min, port_max;
	uint16_t id_min, id_max;
	uint8_t smb_min, smb_max; // trash
} can_net_base_range_t;

// bicycle, 1=TRUE, 0=FALSE
int check_base_range(can_net_base_range_t range, uint8_t port, uint8_t smb, uint16_t id);
inline int check_base_range_meta(can_net_base_range_t range, msg_metadata_t* meta);

// CONST msg_lvl2_t* msg for send_callback:
typedef void (*can_net_send_callback_t)(const int rc, msg_lvl2_t* msg, void * context);
typedef void (*can_net_recv_callback_t)(const msg_lvl2_t* msg, void * context);

// and small bonus:
inline void call_scb(can_net_send_callback_t send_cb, const int rc, msg_lvl2_t* msg, void* cb_ctx);

/*----------------------------------------------------------------------------------------------*/

//return codes for callbacks:
#define CAN_NET_RC_NORM						0
#define CAN_NET_RC_UNKNOWN					1
#define CAN_NET_RC_CONF_TIMEOUT				2
#define CAN_NET_RC_TOO_LONG_MSG				4
#define CAN_NET_RC_BAD_DATA					100
// return codes >THIS are from driver (defined in can_drv.h => are uniq)
#define CAN_NET_RC_DRIVER_ERRS_START		10000

/* -----------------------------------CONFIRMATION_data/struct/funcs----------------------------*/

//TODO:CANNET_REFACTORING make file can_net_confirmations

#ifdef CAN_NET_CONFIRMATION

typedef struct {
	void* next; // to use in list
	msg_lvl2_t* msg;
	uint32_t tics_left; // num of timer_tics else to wait
	can_net_send_callback_t callback;
	void* cb_ctx;
} confirm_waiter_t;

inline void remove_confirm_waiter(confirm_waiter_t* waiter);

inline void add_confirm_waiter(msg_lvl2_t* msg, const uint32_t tics, const can_net_send_callback_t cb, void* cb_ctx);

confirm_waiter_t* find_confirm_waiter(const uint8_t port, const uint32_t hwa, const uint8_t smb, const uint16_t id);

void check_confirm_waiters(void);

#endif

#endif /* CAN_NET_MIDDLE_H_ */
