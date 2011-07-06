// created 19.05.2011
// to be changed later:
/*
 * Copyright (C) 2011 by <Project Master Device>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#ifndef CAN_NET_MIDDLE_H_
#define CAN_NET_MIDDLE_H_

#include "can_net_core.h"
#include "../lib/bytearray.h"

/* defines:
		CAN_NET_CONFIRMATION	- use confirmations on msg
*/
#define CAN_NET_CONFIRMATION

/* -------------------------------------INTERFACE_STRUCTS---------------------------------------*/

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
