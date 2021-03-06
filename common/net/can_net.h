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


/**
 * @file can_net
 * @brief CAN local network system, v0.75
 * fully async
 * with port (instead of apps)
 * with new std ('frame numbering') protocol
 * with err handling: return codes in callbacks
 * with confirmations (for async can_net)
 * with queues of many msgs to one hwa::port::id+smb
 * with adding/removing callbacks
 * TODO: for next ver
 * 			-better queuing(not recursive send)	- v0.77
 * 			-new_protocol						- v0.80
 * 			-more flexible mutexes				- v0.85
 * 			-config, -log						-
 * 			-CAN_err frames handling			-
 * 			-better hearbeat handling			-
 * 			...
 * 			-BIG refactoring					- v2.00
 */

#ifndef CAN_NET_H_
#define CAN_NET_H_

//#define CAN_NET_CONTIKI
//#define CAN_NET_LINUX
//#define CAN_NET_LOWLVL_FUNCS

#ifdef CAN_NET_LINUX
#include <pthread.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/syscall.h>
#endif

#include "can_net_middle.h"


/* -------------------------------------STRUCTS-----------------------------------------*/

// CALLBACKS:
typedef struct {
	can_net_recv_callback_t callback;
	can_net_base_range_t check;
	void* cb_ctx;
} can_net_recv_cb_record_t;

//const can_net_base_range_t can_net_std_range = {0, -1, 0, -1, 0, 1};
extern const can_net_recv_cb_record_t can_net_std_cb_record;

/**
 * @struct can_net_recv_cb_record_t_list
 * @brief  can_net_recv_cb_record_t in list
 */
typedef struct {
	void* next; // to use in list
	can_net_recv_cb_record_t* it_p;
} can_net_recv_cb_record_t_plist;

// OTHER:
typedef struct {
	void* next; // to use in list
	msg_lvl2_t it;
} msg_lvl2_t_list;

typedef struct {
	void* next; // to use in list
	msg_lvl2_t* it_p;
} msg_lvl2_t_plist;

msg_lvl2_t* msg_lvl2_make_copy(const msg_lvl2_t* src);

/*----------------------------------------------------------------------------------------------*/
// heartbeat exception - don't send confirmation on this msg, but they are handled by applications:
#define SYSMSG_HEARTBEAT_ID					0

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
 * @ 1) recv_callbacks: array of callbacks for received msgs - can_net_recv_cbs_arr_t
 * @return: first of inserted callbacks (if inserted only one - this one)- can_net_recv_cb_record_t*
 * uses your memory, doesn't make copy of callback
 */
can_net_recv_cb_record_t_plist* can_net_add_callback(can_net_recv_cb_record_t* callback_p);

/**
 * can_net_del_callback_by_p - remove callback from net system (by its pointer)
 * @ 1) callback_p: pointer (in list) to required callback - can_net_recv_cb_record_t
 */
void can_net_rm_callback_by_plist(can_net_recv_cb_record_t_plist* callback_plist);

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
 * uses it's own memory, copies msg
 */
void can_net_start_sending_msg(const msg_lvl2_t* msg, can_net_send_callback_t send_callback, void* cb_ctx);

/* ----------------------------------------------------------------------------------------------*/
//#ifdef CAN_NET_LOWLVL_FUNCS
//#endif

#endif /* CAN_NET_H_ */
