/*
 * can_net_sync_wrapper.c
 *
 *  Created on: Jun 3, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#include "can_net_sync_wrapper.h"

int can_net_send_msg_sync(/*const*/ msg_lvl2_t* msg) {
	//TODO: can_net_send_msg_sync
	/*callback: { x = 0 }
	 * x = 1
	 * while(x) { sleep(); }
	 * return
	 */
}

msg_lvl2_t* can_net_read_msg_sync() {
	//TODO: can_net_read_msg_sync
	/*callback: { y = 0; msg_read = mgs_received //(make copy) }
	 * y = 1
	 * while(y) { sleep(); }
	 * return msg_read
	 */
}
