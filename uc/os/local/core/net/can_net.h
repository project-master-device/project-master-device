// 03.03.2011
// 20.05.2011
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

/**
 * @file can_net
 * @brief CAN local network system, v0.6
 * fully async
 * with port (instead of apps)
 * with new std ('frame numbering') protocol
 * with err handling: return codes in callbacks
 * with confirmations (for async can_net)
 * with queues of many msgs to one hwa::port::id+smb
 * TODO: for next ver
 * 			-refactoring						- to v0.80
 * 			-more flexible mutexes				- to v0.80
 * 			-config, -log						-
 * 			-CAN_err frames handling			-
 */
//TODO: make comments BETTER

#ifndef CAN_NET_H_
#define CAN_NET_H_

#include <stdlib.h>
#include "list.h"
#include "can_protocols.h"
#include "can_drv.h"

/* defines:
		CAN_NET_VALIDATE_MSGS	- calls _validate before _pack (from can_protocols)
		CAN_NET_CONFIRMATION	- use confirmations on msg
									(works bad when testing can_net one one machine,- only with old can_socket
		CAN_NET_QUEUING			- use queuing (allows fast msgs sending in right order)
		CAN_NET_LINUX			- for linux-only code (like mutexes)
//		CAN_NET_LOWLVL_FUNCS	- low-level functions to test errors (not avaliable at the moment)
*/
#define CAN_NET_VALIDATE_MSGS
//#define CAN_NET_CONFIRMATION
//#define CAN_NET_QUEUING
#define CAN_NET_CONTIKI
//#define CAN_NET_LINUX
//#define CAN_NET_LOWLVL_FUNCS

#ifdef CAN_NET_LINUX
#include <pthread.h>
#include <unistd.h>
#endif

/*----------------------------------------------------------------------------------------------*/

/**
 * @struct bytearr_t
 * @brief simple byte array
 */
typedef struct {
	uint8_t* itself;
	uint32_t len;
} bytearr_t;

/**
 * @struct msg_metadata_t
 * @brief header for message
 */
typedef struct {
	// TODO: add port here
	uint32_t hw_addr;
	uint8_t is_system;
	uint16_t id;
} msg_metadata_t;

typedef struct {
	// TODO: or here
	msg_metadata_t meta;
	bytearr_t data;
} msg_lvl2_t;

/*----------------------------------------------------------------------------------------------*/

typedef struct {
	uint8_t port_min, port_max;
	uint16_t id_min, id_max;
	uint8_t smb_min, smb_max; // trash
} can_net_base_range_t;

typedef void (*can_net_send_callback_t)(const int rc, msg_lvl2_t* msg);
typedef void (*can_net_recv_callback_t)(msg_lvl2_t* msg, const int port);

typedef struct {
	can_net_recv_callback_t callback;
	can_net_base_range_t check;
} can_net_recv_callback_record_t;

/**
 * @struct can_net_recv_callbacks_arr_t
 * @brief array of callback records
 */
typedef struct {
	can_net_recv_callback_record_t* records;
	int len;
} can_net_recv_callbacks_arr_t;
#define CAN_NET_RECV_CALLBACKS_ARR_INITIALIZER {NULL, 0}

/*----------------------------------------------------------------------------------------------*/

//return codes for callbacks:
#define CAN_NET_RC_NORM						0
#define CAN_NET_RC_UNKNOWN					1
#define CAN_NET_RC_CONF_TIMEOUT				2
#define CAN_NET_RC_TOO_LONG_MSG				4
#define CAN_NET_RC_BAD_DATA					100
// return codes >THIS are from driver (defined in can_drv.h => are uniq)
#define CAN_NET_RC_DRIVER_ERRS_START		10000

/*----------------------------------------------------------------------------------------------*/

#ifdef CAN_NET_LINUX
#define CAN_NET_CONFIRMATION_TIC_TIME_US	1000 // 1ms
#endif

#ifdef CAN_NET_CONTIKI
#define CAN_NET_CONFIRMATION_TIC_TIME_US    10000 // 10ms
    #if CAN_NET_CONFIRMATION_TIC_TIME_US < 8000
    #error CAN_NET_CONFIRMATION_TIC_TIME_US is not correct. The value must be greater than or equal 8000.
    #endif
#endif

#define CAN_NET_MAX_MSG_SIZE					CAN_NET_SEGM_FNL_MAX_NUM * CAN_NET_SEGM_FNL_DATALEN_B

#define CAN_NET_PORT_BROADCAST					0
#define CAN_NET_LVL2_VER_STD					1
#define CAN_NET_LVL2_VER_STD_CONFIRM			2

/*----------------------------------------------------------------------------------------------*/

/**
 * can_net_app_join - add your app callbacks to can_net system
 * @ 1) recv_callbacks: array of callbacks for received msgs - can_net_recv_callbacks_arr_t
 */
void can_net_add_callbacks(can_net_recv_callbacks_arr_t recv_callbacks);

/**
 * can_net_start_work - initialize network system (needs interface & period => exists until config is made)
 * @ 1) send_frame_timeout_u
				// code copying, but nothing can fix it, right?
				#ifdef CAN_NET_QUEUING
				if (atom->sending == 0) {
					// here we are sure it's not reading
				} else
					atom->is_receiving = 0;
				#else
				list_remove(can_node->atoms, atom);
				atom_free(atom);
				#endif
 * s:  (should be > ~3*period)- const uint32_t
 * @ 2) confirmation_tics_: tics to wait for confirmation frame after sending msg (time=tics*tic_period) - const uint32_t
 * @return: return codes: pthread problem: 2, can_socket problem: 1, already initialized: 4
 */
int can_net_init(const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics_);

/**
 * can_net_start_sending_msg - start message sending
 * @ 1) msg: message to send - const msg_lvl2_t*
 * @ 2) app_code: your application code (don't lie!) - const int
 */
void can_net_start_sending_msg(/*const*/ msg_lvl2_t* msg, const uint8_t port, can_net_send_callback_t send_callback);

/* ----------------------------------------------------------------------------------------------*/
//#ifdef CAN_NET_LOWLVL_FUNCS
//#endif

#endif /* CAN_NET_H_ */
