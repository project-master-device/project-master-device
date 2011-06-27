/*
 * can_net_sync_wrapper.c
 *
 *  Created on: Jun 3, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#include "can_net_sync_wrapper.h"

typedef struct {
	int send_is_sending;
	int send_rc;
} sender_descriptor_t;

void sync_wr_send_callback(const int rc, msg_lvl2_t* msg, void* ctx) {
	sender_descriptor_t* descr = (sender_descriptor_t*)ctx;
	descr->send_is_sending = 0;
	descr->send_rc = rc;
}

void sync_wr_recv_callback(msg_lvl2_t* msg, void* ctx) {
	int* recv_is_recieving = (int*)ctx;
	*recv_is_recieving = 0;
}

int can_net_sync_init(uint8_t port, const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics) {
	if (port > CAN_NET_PORT_MAX)
		return -1;
	can_net_recv_callbacks_arr_t recv_callbacks;
	recv_callbacks.len = 1;
	recv_callbacks.records = (can_net_recv_callback_record_t*)malloc(sizeof(can_net_recv_callback_record_t) * recv_callbacks.len);
	recv_callbacks.records[0] = can_net_std_cb_record;
	recv_callbacks.records[0].callback = sync_wr_recv_callback;
	recv_callbacks.records[0].check.port_min = port;
	recv_callbacks.records[0].check.port_max = port;
	can_net_add_callbacks(recv_callbacks);
	int rci = can_net_init(send_frame_timeout_us, confirmation_tics);
	return rci;
}

int can_net_sync_send(/*const*/ msg_lvl2_t* msg) {
	int rc = -1;
	if (msg->meta.port < CAN_NET_PORT_MAX) {
		sender_descriptor_t descr;
		descr.send_is_sending = 1;
		descr.send_rc = 0;

		can_net_start_sending_msg(msg, sync_wr_send_callback, (void*)&descr);

		while(descr.send_is_sending) {
			usleep(CAN_NET_SYNCWR_SEND_CHT_US);
		}
		rc = descr.send_rc;
	}
	return rc;
}

int can_net_sync_recv(msg_lvl2_t* msg) {
	//TODO: can_net_read_msg_sync
	/*callback: { y = 0; msg = mgs_received //(make copy) }
	 * y = 1
	 * while(y) { sleep(); }
	 * return msg_read
	 */
	return 0;
}
