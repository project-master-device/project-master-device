/*
 * can_net_sync_wrapper.h
 *
 *  Created on: Jun 3, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#ifndef CAN_NET_SYNC_WRAPPER_H_
#define CAN_NET_SYNC_WRAPPER_H_

#include <pthread.h>
#include "../../common/net/can_net.h"

#define CAN_NET_SYNCWR_SEND_CHT_US 1 * 1000 // 1ms
#define CAN_NET_SYNCWR_RECV_CHT_US 1 * 1000 // 1ms

/**
 * can_net_start_sending_msg - start message sending
 * @ 1) send_frame_timeout_us - look at can_net::can_net_init() - const uint32_t
 * @ 2) confirmation_tics - look at can_net::can_net_init() - const uint32_t
 * @return: return codes:  - look at can_net::can_net_init() - int
 */
int can_net_sync_init(uint8_t port, const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics);

/**
 * can_net_send_msg_sync - start message sending
 * @ 1) msg: message to send - const msg_lvl2_t*
 * @return: return codes: look at callback return codes in can_net_middle.h
 */
int can_net_sync_send(/*const*/ msg_lvl2_t* msg);

/**
 * can_net_start_sending_msg - start message sending
 * @ 1) msg: ptr to message - msg_lvl2_t*
 * @return: return codes: 0: timeout
 */
int can_net_sync_recv(msg_lvl2_t* msg);

#endif /* CAN_NET_SYNC_WRAPPER_H_ */
