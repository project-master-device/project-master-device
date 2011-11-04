/*
 * can_net_sync_wrapper.c
 *
 *  Created on: Jun 3, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#include "can_net_sync_wrapper.h"

#define SEND_CHT_US			1*1000 // 1ms
#define RECV_CHT_US			1*1000 // 1ms

#warning "Sync Wrapper: broadcast port isnt real broadcast - msg is extracted by first app - fix before using >1 apps"
#ifndef CAN_NET_LINUX
	#error "Never tested on anything but linux (also uses linux-mutexes)"
#endif

typedef struct {
	int is_sending;
	int rc;
	int actual;
	pthread_mutex_t mutex;
} sender_descriptor_t;

// port lvl msgs buffer:
typedef struct {
	int msgs_num;
	int msgs_limit;
	int timeout_cycles;
	LIST_STRUCT(msgs);
	can_net_recv_cb_record_t_plist* cb_plist; // useless at the moment (17.07.11)
} port_buffer_t;
port_buffer_t* ports[CAN_NET_PORT_MAX] = { NULL };
pthread_mutex_t ports_mutex = PTHREAD_MUTEX_INITIALIZER;

inline void free_descr(sender_descriptor_t* descr) {
	pthread_mutex_unlock(&descr->mutex);
	pthread_mutex_destroy(&descr->mutex);
	free(descr);
}

// should be called sooner or later, if not called - ctx memory becomes leak
void sync_wr_send_callback(const int rc, msg_lvl2_t* msg, void* ctx) {
	sender_descriptor_t* descr = (sender_descriptor_t*)ctx;
	pthread_mutex_lock(&descr->mutex);
	if (descr->actual == 1) {
		descr->is_sending = 0;
		descr->rc = rc;
		pthread_mutex_unlock(&descr->mutex);
	} else {
		//free_descr(descr);
		pthread_mutex_unlock(&descr->mutex);
	}
}

void sync_wr_recv_callback(const msg_lvl2_t* msg, void* ctx) {
	pthread_mutex_lock(&ports_mutex);

	if ( (ports[msg->meta.port]->msgs_num < ports[msg->meta.port]->msgs_limit) || (ports[msg->meta.port]->msgs_limit < 0) ) {
		ports[msg->meta.port]->msgs_num++;
		msg_lvl2_t_plist* msg_pl = (msg_lvl2_t_plist*)malloc(sizeof(msg_lvl2_t_plist));
		msg_pl->it_p = msg_lvl2_make_copy(msg);
		list_add(ports[msg->meta.port]->msgs, msg_pl);
	}

	pthread_mutex_unlock(&ports_mutex);
}

int can_net_sync_init(const uint8_t port, const int msgs_limit, const int timeout_cycles,
		const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics) {
	ports[port] = (port_buffer_t*)malloc(sizeof(port_buffer_t));
	ports[port]->msgs_num = 0;
	ports[port]->msgs_limit = msgs_limit;
	ports[port]->timeout_cycles = timeout_cycles;
	LIST_STRUCT_INIT(ports[port], msgs);

	can_net_recv_cb_record_t* cbr_p = (can_net_recv_cb_record_t*)malloc(sizeof(can_net_recv_cb_record_t));
	*cbr_p = can_net_std_cb_record;
	cbr_p->callback = sync_wr_recv_callback;
	cbr_p->cb_ctx = NULL;
	cbr_p->check.port_min = port;
	cbr_p->check.port_max = port;
	ports[port]->cb_plist = can_net_add_callback(cbr_p);

	int rci = can_net_init(send_frame_timeout_us, confirmation_tics);
	return rci;
}

int can_net_sync_send(const msg_lvl2_t* msg) {
	int rc = -1;
	if (msg->meta.port < CAN_NET_PORT_MAX) {
		// TODO: FIX TIMEOUT=>SEGFAULT BUG // fixed 06.2011?
		sender_descriptor_t* descr = (sender_descriptor_t*)malloc(sizeof(sender_descriptor_t));
		descr->is_sending = 1;
		descr->rc = 1;
		descr->actual = 1;
		pthread_mutex_init(&descr->mutex, NULL);

		can_net_start_sending_msg(msg, sync_wr_send_callback, (void*)descr);

		int cycles_spent = 0;
		while( ((cycles_spent < ports[msg->meta.port]->timeout_cycles) && (descr->is_sending)) ||
				(ports[msg->meta.port]->timeout_cycles < 0) ) { // no timeout = when it's done
			usleep(SEND_CHT_US);
			cycles_spent++;
		}
		pthread_mutex_lock(&descr->mutex);
		rc = descr->rc;
		if (descr->is_sending) {
			// quit on timeout
			descr->actual = 0;
			pthread_mutex_unlock(&descr->mutex);
		} else {
			// quit on callback
			free_descr(descr);
		}
	}
	return rc;
}


// WARNING: possibly govnokod; TODO: think more - fixed?
int can_net_sync_recv(const uint8_t port, msg_lvl2_t** msg) {
	int rc = -1; //impossible rc
	if ((port < CAN_NET_PORT_MAX) && (ports[port] != NULL)) {
		msg_lvl2_t_plist* msg_pl;
		int timeout_counter = 0;
		while(1) {
			// try to get msg:
			pthread_mutex_lock(&ports_mutex);
			msg_pl = list_pop(ports[port]->msgs);
			pthread_mutex_unlock(&ports_mutex);
			if (msg_pl != NULL) {
				// quit on receiving:
				*msg = msg_pl->it_p;
				free(msg_pl);
				pthread_mutex_lock(&ports_mutex);
				ports[(*msg)->meta.port]->msgs_num--;
				pthread_mutex_unlock(&ports_mutex);
				rc = 0;
				break;
			}
			if ( (timeout_counter > ports[port]->timeout_cycles) && (ports[port]->timeout_cycles >= 0) ) {
				// quit on timeout:
				*msg = NULL;
				rc = 1;
				break;
			}
			usleep(RECV_CHT_US);
			timeout_counter++;
		}
	}
	return rc;
}


int can_net_sync_close(const uint8_t port) {
//	TODO: can_net_core.c::38
//	can_net_rm_callback_by_plist(ports[port]->cb_plist);
//	list_free_all(ports[port]->msgs); -- WILL LEAK
//	free(ports[port]);
	return 0;
}
