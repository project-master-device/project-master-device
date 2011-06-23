/*
 * can_net_sync_wrapper.h
 *
 *  Created on: Jun 3, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#ifndef CAN_NET_SYNC_WRAPPER_H_
#define CAN_NET_SYNC_WRAPPER_H_

#include "../../../../net/trunk/can_net.h"

/**
 * can_net_send_msg_sync - start message sending
 * @ 1) msg: message to send - const msg_lvl2_t*
 * @return: return codes: look at callback return codes in can_net_middle.h
 */
int can_net_send_msg_sync(/*const*/ msg_lvl2_t* msg);

/**
 * can_net_start_sending_msg - start message sending
 * @return: return codes: look at callback return codes in can_net_middle.h
 */
msg_lvl2_t* can_net_read_msg_sync();

#endif /* CAN_NET_SYNC_WRAPPER_H_ */
