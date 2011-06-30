//19.04.2011
//12.05.2011

#include <pthread.h> // -lpthread
//#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "../../../common/net/list.h"
#include "../../../common/net/can_drv.h"
#include "can_socket.h"

pthread_mutex_t frames_out_mutex = PTHREAD_MUTEX_INITIALIZER;

#define CAN_DRV_CYCLE_PERIOD_US 100

LIST(frames_out);

typedef struct timeval timeval_t;
typedef struct {
	void* next;
	can_frame_t* can_frame;
	timeval_t timeout_time;
	void* context;
} frame_descr_t;

#ifndef CAN_DRV_CYCLE_PERIOD_US
#error CAN_DRV_CYCLE_PERIOD_US_NOT_SET
#endif

void* drv_main_cycle(void* p);
const char* device = "can0";	// !
can_send_callback_t send_callback;
can_recv_callback_t recv_callback;
int send_timeout_us;
int can_socket;
int can_init(can_speed_t can_speed, can_send_callback_t send_callback_, can_recv_callback_t recv_callback_,
		uint32_t send_timeout_real_us , can_mask_t masked_id, can_mask_t mask) {
	can_socket = can_cs_init_socket(device);
	if (can_socket < 0) {
		return 1;
	}
	send_callback = send_callback_;
	recv_callback = recv_callback_;
	send_timeout_us = send_timeout_real_us;
//	can_set_mask(masked_id, mask);
//	if ( can_set_speed(can_speed) ) {
//		return 3;
//	}
	pthread_t thread;
	int iret = pthread_create(&thread, NULL, drv_main_cycle, NULL);
	if (!iret)
		return 0;
	else
		return 2;
}

void* drv_main_cycle(void* p) {
	timeval_t now;
	frame_descr_t* frame_descr;
	struct can_frame cs_frame;
	can_frame_t frame;
	int bytes_sent;
	int rc_s = 0;
	while (1) {
		// send:
		pthread_mutex_lock(&frames_out_mutex);
		frame_descr = list_head(frames_out);
		pthread_mutex_unlock(&frames_out_mutex);
		if (frame_descr != NULL) {
			// transform our frame into can_socket frame:
			if( !can_frame_inf_pack(&cs_frame.can_id,
									frame_descr->can_frame->inf.id, frame_descr->can_frame->inf.err,
									frame_descr->can_frame->inf.rtr, frame_descr->can_frame->inf.ide) ) {

				cs_frame.can_dlc = frame_descr->can_frame->data.len;
				memcpy(cs_frame.data, frame_descr->can_frame->data.itself, cs_frame.can_dlc);

				bytes_sent = can_cs_send_frame(can_socket, &cs_frame);
				if (bytes_sent == sizeof(struct can_frame)) {
					rc_s = CAN_DRV_RC_NORM;
					pthread_mutex_lock(&frames_out_mutex);
					list_remove(frames_out, frame_descr);
					pthread_mutex_unlock(&frames_out_mutex);
				} else {
					if ( !gettimeofday(&now, NULL) ) {
						if ( !(timercmp(&frame_descr->timeout_time, &now, >=)) ) {
							rc_s = CAN_DRV_RC_TIMEOUT;
						} //else { do nothing }
					} else {
						rc_s = CAN_DRV_RC_TIMER_ERR;
					}
				}
			} else {
				rc_s = CAN_DRV_RC_WRONG_DATA;
			}
			// rc is always updated!
			send_callback(rc_s, frame_descr->can_frame, frame_descr->context);
			free(frame_descr);
		}

		// receive:
		int rc_v = can_cs_try_receive_frame(can_socket, &cs_frame);
		if (!rc_v) {
			// transform can_socket frame into our frame
			frame.data.len = cs_frame.can_dlc;
			memcpy(frame.data.itself, cs_frame.data, cs_frame.can_dlc);
			frame.inf = can_frame_inf_unpack(cs_frame.can_id);
			recv_callback(&frame);
		}
		else {
			if (rc_v == 2) {} // panic - smth wrong with socket
		}
		// sleep:
		usleep(CAN_DRV_CYCLE_PERIOD_US);
	}
	return NULL;
}

#define MAX_TIMEVAL_USEC 1000000
int can_send(can_frame_t* can_frame, void* context) {
	frame_descr_t* frame_descr = (frame_descr_t*)malloc(sizeof(frame_descr_t));
	if ( gettimeofday(&frame_descr->timeout_time, NULL) )
		return 1;
	// set time of putting to send queue:
	frame_descr->timeout_time.tv_usec += send_timeout_us;
	if (frame_descr->timeout_time.tv_usec >= MAX_TIMEVAL_USEC) {
		frame_descr->timeout_time.tv_usec -= MAX_TIMEVAL_USEC;
		frame_descr->timeout_time.tv_sec++;
	}
	frame_descr->can_frame = can_frame;
	frame_descr->context = context;
	frame_descr->next = NULL;
	pthread_mutex_lock(&frames_out_mutex);
	list_add(frames_out, frame_descr);
	pthread_mutex_unlock(&frames_out_mutex);
	return 0;
}
#undef MAX_TIMEVAL_USEC

void can_set_mask(can_mask_t masked_id, can_mask_t mask) {
	// no need to set mask on master
}

int can_set_speed(can_speed_t can_speed) {
	// do smth. with can_speed:
	// DEVICE = can0, // SPEED = can_speed (or switch by enum)
	// ip link set DEVICE type can bitrate SPEED
	return 1;
}
