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
#define CAN_NET_SYNC_SEND_RC_INVALID_PORT	-1

/**
 * can_net_sync_recv - recv next message
 * @ 1) port: port - uint8_t
 * @ 2) msg: ptr to ptr to message - msg_lvl2_t**
 * @return: return codes: 0: ok, 1: wrong port, 2: timeout
 * tries to take from port buffer, if nothing there - waits until at least one appears in buffer
 * can net sync wrapper will allocate memory for msg automatically, give it just pointer
 */
int can_net_sync_recv(const uint8_t port, msg_lvl2_t** msg);
#define CAN_NET_SYNC_RECV_RC_TIMEOUT	1

/**
 * can_net_sync_close - close, stop listening for incoming messages
 * @ 1) port: port - uint8_t
 * @return: return codes: 0: ok, else: smth wrong happend
 * if called while other (recv, send, init) funcs are working they will segfault (most probably)
 */
int can_net_sync_close(const uint8_t port);

#endif /* CAN_NET_SYNC_WRAPPER_H_ */
