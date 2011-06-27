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
 * @brief CAN local network system, v0.7
 * fully async
 * with port (instead of apps)
 * with new std ('frame numbering') protocol
 * with err handling: return codes in callbacks
 * with confirmations (for async can_net)
 * with queues of many msgs to one hwa::port::id+smb
 * TODO: for next ver
 * 			-new_protocol						- v0.80
 * 			-more flexible mutexes				- v0.85
 * 			-config, -log						-
 * 			-CAN_err frames handling			-
 * 			...
 * 			-BIG refactoring					- v2.00
 */
//TODO: make comments BETTER

#ifndef CAN_NET_H_
#define CAN_NET_H_

//#define CAN_NET_CONTIKI
#define CAN_NET_LINUX
//#define CAN_NET_LOWLVL_FUNCS

#ifdef CAN_NET_LINUX
#include <pthread.h>
#include <unistd.h>
#endif

#include "can_net_middle.h"

/* -------------------------------------CALLBACKS-----------------------------------------------*/

typedef struct {
	can_net_recv_callback_t callback;
	can_net_base_range_t check;
	void* cb_ctx;
} can_net_recv_callback_record_t;

//const can_net_base_range_t can_net_std_range = {0, -1, 0, -1, 0, 1};
extern const can_net_recv_callback_record_t can_net_std_cb_record;

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
#define CAN_NET_PORT_MAX						15// 0xf
#define CAN_NET_LVL2_VER_STD					1
#define CAN_NET_LVL2_VER_STD_CONFIRM			2

/*-----------------------------------------INTERFACE--------------------------------------------*/

/**
 * can_net_app_join - add your app callbacks to can_net system
 * @ 1) recv_callbacks: array of callbacks for received msgs - can_net_recv_callbacks_arr_t
 */
void can_net_add_callbacks(can_net_recv_callbacks_arr_t recv_callbacks);

/**
 * can_net_start_work - initialize network system (needs interface & period => exists until config is made)
 * @ 1) send_frame_timeout_us (should be > ~3*period)- const uint32_t
 * @ 2) confirmation_tics_: tics to wait for confirmation frame after sending msg (time=tics*tic_period) - const uint32_t
 * @return: return codes: pthread problem: 2, can_socket problem: 1, already initialized: 4
 * the first program that starts can_net will set send_frame_timeout_us and confirmation_tics_
 */
int can_net_init(const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics_);

/**
 * can_net_start_sending_msg - start message sending
 * @ 1) msg: message to send - const msg_lvl2_t*
 * @ 2) send_callback: callback to be called after sending msg
 * without CAN_NET_QUEUING you should not send msgs too often
 */
void can_net_start_sending_msg(/*const*/ msg_lvl2_t* msg, can_net_send_callback_t send_callback, void* cb_ctx);

/* ----------------------------------------------------------------------------------------------*/
//#ifdef CAN_NET_LOWLVL_FUNCS
//#endif

#endif /* CAN_NET_H_ */
