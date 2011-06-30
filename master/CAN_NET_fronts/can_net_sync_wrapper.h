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

/**
 * can_net_sync_init - initialize, start listening for incoming messages
 * @ 1) port: port - uint8_t
 * @ 2) msgs_limit: msgs limit - int
 * @ 3) timeout_cycles: timeout cycles - int
 * @ 4) send_frame_timeout_us: look at can_net::can_net_init() - const uint32_t
 * @ 5) confirmation_tics: look at can_net::can_net_init() - const uint32_t
 * @return: return codes: look at can_net::can_net_init() - int
 */
int can_net_sync_init(const uint8_t port, const int msgs_limit, const int timeout_cycles,
		const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics);

/**
 * can_net_sync_send - send message
 * @ 1) msg: message to send - const msg_lvl2_t*
 * @return: return codes: look at callback return codes in can_net_middle.h
 */
int can_net_sync_send(const msg_lvl2_t* msg);

/**
 * can_net_sync_recv - recv next message
 * @ 1) port: port - uint8_t
 * @ 2) msg: ptr to ptr to message - msg_lvl2_t**
 * @return: return codes: 0: ok, 1: wrong port, 2: timeout
 * tries to take from port buffer, if nothing there - waits until at least one appears in buffer
 * can net sync wrapper will allocate memory for msg automatically, give it just pointer
 */
int can_net_sync_recv(const uint8_t port, msg_lvl2_t** msg);

/**
 * can_net_sync_close - close, stop listening for incoming messages
 * @ 1) port: port - uint8_t
 * @return: return codes: 0: ok, else: smth wrong happend
 * if called while other (recv, send, init) funcs are working they will segfault (most probably)
 */
int can_net_sync_close(const uint8_t port);

#endif /* CAN_NET_SYNC_WRAPPER_H_ */
