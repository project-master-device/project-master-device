// created 04.03.2011
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

#include "can_net.h"

//OTHER:
msg_lvl2_t* msg_lvl2_make_copy(const msg_lvl2_t* src) {
	msg_lvl2_t* dst = (msg_lvl2_t*)malloc(sizeof(msg_lvl2_t));
	*dst = *src;
	dst->data.len = src->data.len; // is necessary?
	dst->data.itself = (uint8_t*)malloc(dst->data.len);
	memcpy(dst->data.itself, src->data.itself, dst->data.len);
	return dst;
}

/* ----------------------------------------LIST_STRUCTS-----------------------------------------*/

const can_net_recv_cb_record_t can_net_std_cb_record = {NULL, {0, -1, 0, -1, 0, 1}, NULL};

typedef struct {
	void* next; // to use in list
	can_frame_data_t it;
} frame_data_t_list;

typedef struct {
	void* next; // to use in list
	int it;
} error_t_list;

typedef struct {
	void* next; // to use in list
	can_frame_t it;
} can_frame_t_list;

/* ---------------------------------------------DATA--------------------------------------------*/

//can_net_recv_cbs_arr_t recv_callbacks = CAN_NET_RECV_CALLBACKS_ARR_INITIALIZER;
LIST(recv_callbacks);

#ifdef CAN_NET_LINUX
	#define MUTEX_L(mutex_p) pthread_mutex_lock(mutex_p);
	#define MUTEX_U(mutex_p) pthread_mutex_unlock(mutex_p);
	pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t recv_callbacks_mutex = PTHREAD_MUTEX_INITIALIZER;
	#ifdef CAN_NET_CONFIRMATION
		pthread_mutex_t confirm_waiters_mutex = PTHREAD_MUTEX_INITIALIZER;
	#endif

	#ifndef __NR_gettid
	#  define __NR_gettid	224
	#endif

	pid_t can_net_thread_id;
#endif

#ifdef CAN_NET_CONTIKI
    #ifdef CAN_NET_CONFIRMATION
    #include "lib/ftimer.h"
    #endif
	#define MUTEX_L(mutex_p)
	#define MUTEX_U(mutex_p)
#endif

/*----------------------------------------------INIT-------------------------------------------*/

can_net_recv_cb_record_t_plist* can_net_add_callback(can_net_recv_cb_record_t* callback_p) {
	// safe when called from callback (can_net thread)
	// NOT safe when called from another thread (not can_net thread) - protected with mutex

	int thread_safe = 0;
	#ifdef CAN_NET_LINUX
	pid_t this_thread_id = (pid_t)syscall(__NR_gettid); //gettid()
	if (this_thread_id == can_net_thread_id) {
		// expected only to be called from callback, make sure there is no calls from other parts of can_net
		thread_safe = 1;
	}
	#endif

	if (!thread_safe)
		MUTEX_L(&recv_callbacks_mutex);

	can_net_recv_cb_record_t_plist* new_cb = (can_net_recv_cb_record_t_plist*)malloc(sizeof(can_net_recv_cb_record_t_plist));
	list_add(recv_callbacks, new_cb);
	new_cb->it_p = callback_p;

	if (!thread_safe)
		MUTEX_U(&recv_callbacks_mutex);
	return new_cb;
}

void can_net_rm_callback_by_plist(can_net_recv_cb_record_t_plist* callback_plist) {
	// TODO: allow callback to delete itself, (and maybe someone else (question of security)) - NOT ALLOWED AT THE MOMENT
	// NOT safe when called from another thread (not can_net thread) - protected with mutex

//	int thread_safe = 0;
	#ifdef CAN_NET_LINUX
	pid_t this_thread_id = (pid_t)syscall(__NR_gettid); //gettid()
	if (this_thread_id == can_net_thread_id) {
//		// expected only to be called from callback, make sure there is no calls from other parts of can_net
//		thread_safe = 1;
		return;
	}
	#endif

//	if (!thread_safe)
	MUTEX_L(&recv_callbacks_mutex)

	list_remove(recv_callbacks, (void*)callback_plist);
	free(callback_plist);

//	if (!thread_safe)
	MUTEX_U(&recv_callbacks_mutex)
}

#ifdef CAN_NET_CONFIRMATION
	uint32_t confirmation_tics_g;
	#ifdef CAN_NET_LINUX
	void* check_confirm_waiters_cycle(void* p) {
		while(1) {
			MUTEX_L(&confirm_waiters_mutex)
			check_confirm_waiters();
			MUTEX_U(&confirm_waiters_mutex)
			// sleep:
			usleep(CAN_NET_CONFIRMATION_TIC_TIME_US);
		}
	}
	#endif
	#ifdef CAN_NET_CONTIKI
	void check_confirm_waiters_cb(void * p) {
		check_confirm_waiters();
		ftimer_register_func(check_confirm_waiters_cb, 0, CAN_NET_CONFIRMATION_TIC_TIME_US / 1000);
	}
	#endif
#endif

// callbacks for can drv:
void msg_send_handler(const int drv_rc, can_frame_t* frame, void* context_void);
void frame_recv_handler(can_frame_t *);

int is_initialezed = 0;
// INTERFACE:
int can_net_init(const uint32_t send_frame_timeout_us, const uint32_t confirmation_tics_) {
	if (is_initialezed)
		return 4;
	is_initialezed = 1;

	// start can driver:
	can_send_callback_t send_callback = msg_send_handler;
	can_recv_callback_t recv_callback = frame_recv_handler;
	can_speed_t can_speed = CAN_125;
	int rc = can_init(can_speed, send_callback, recv_callback, send_frame_timeout_us, 0, 0);
	if (rc)
		return rc; // 1-cansocket, 2-pthread

	#ifdef CAN_NET_CONFIRMATION
	// start something(platform dependent) that will check confirmation waiters timeout
		confirmation_tics_g = confirmation_tics_;
		#ifdef CAN_NET_LINUX
			pthread_t conf_thread;
			int iret = pthread_create(&conf_thread, NULL, check_confirm_waiters_cycle, NULL);
			if (iret)
				return 2;
		#endif
		#ifdef CAN_NET_CONTIKI
			ftimer_register_func(check_confirm_waiters_cb, 0, CAN_NET_CONFIRMATION_TIC_TIME_US / 1000);
		#endif
	#endif
	return 0;
}

/*----------------------------------------------OUT--------------------------------------------*/

//#define VERSION_IN_USE	CAN_NET_LVL2_VERSION_STD
int can_frame_pack(can_frame_t* packed, const uint8_t ver, const uint32_t hw_addr) {
	packed->inf.err = 0;
	packed->inf.rtr = 0;
	if (hw_addr > CAN_NET_LVL1_MAX_HWA_NOT_EXT)
		packed->inf.ide = 1;
	else
		packed->inf.ide = 0;

	if ( frame_head_pack(&packed->inf.id, ver, hw_addr, packed->inf.ide) )
		return 1;
	return 0;
}

// asynchronous functions to be call with !context!
typedef struct {
	uint8_t first_iteration;

	uint8_t num;
	uint8_t* data_p;
	uint32_t bytes_left;
	uint8_t datalen;
	uint8_t lfb;
} segmentator_num_lfb_context_t;

#define STD_DATALEN			CAN_NET_SEGM_FNL_DATALEN_B
int segmentator_num_lfb_async(can_frame_t* new_frame, msg_lvl2_t* msg, segmentator_num_lfb_context_t* ctx) {
	if (ctx->first_iteration == 1) {
		if (msg->data.len > CAN_NET_MAX_MSG_SIZE)
			return 2; // msg is too long

		ctx->num = 0;
		ctx->data_p = msg->data.itself;
		ctx->bytes_left = msg->data.len;
		ctx->datalen = STD_DATALEN;
//		ctx->lfb = 0;
		ctx->first_iteration = 0;
	}
	if (ctx->bytes_left <= STD_DATALEN) {
		// last frame:
		ctx->datalen = ctx->bytes_left;
		ctx->lfb = 1;
	}
	if ( lvl_segm_fnl_pack(&new_frame->data, ctx->lfb, ctx->num, ctx->data_p, ctx->datalen) )
		return 1; // should never happen
	if (!ctx->lfb) {
		ctx->bytes_left -= STD_DATALEN;
		ctx->data_p += STD_DATALEN;
		ctx->num += 1;
	}
	return 0;
}
#undef STD_DATALEN

// asynchronous function to be call with !context!
typedef struct {
	msg_lvl2_t* msg;
	uint16_t base;
	uint8_t first_iteration;
	can_net_send_callback_t callback;
	void* cb_ctx;
	segmentator_num_lfb_context_t* segm_ctx;
} send_msg_context_t;

// finish Msg_Send_Handler:
void msh_finish(const int rc_up, send_msg_context_t* ctx) {
//	MUTEX_L(&data_mutex) - why is it here?
	int call_cb = 1;

    #ifdef CAN_NET_CONFIRMATION
	if ( (ctx->msg->meta.is_system != 1) || (ctx->msg->meta.id != SYSMSG_HEARTBEAT_ID) ) {
	    // not heartbeat:
	    if ( (rc_up == CAN_NET_RC_NORM))
	        call_cb = 0;
    }
    #endif

	if (call_cb)
		call_scb(ctx->callback, rc_up, ctx->msg, ctx->cb_ctx);
//	MUTEX_U(&data_mutex)

	free(ctx->segm_ctx);
	free(ctx);
}

typedef struct {
	void* next; // to use in list
	send_msg_context_t* ctx;
} msgs_queue_t;

void msg_send_handler(const int drv_rc, can_frame_t* frame, void* context_void) {
	if (frame != NULL) free(frame);
	if (context_void == NULL) {
		// msg was a confirmation (or smth else special)
		return;
	}
	send_msg_context_t* ctx = (send_msg_context_t*)context_void;
	if (drv_rc == CAN_DRV_RC_NORM) {
		if (ctx->first_iteration) {
			ctx->base = 0;
			ctx->first_iteration = 0;
			ctx->segm_ctx = (segmentator_num_lfb_context_t*)malloc(sizeof(segmentator_num_lfb_context_t));
			ctx->segm_ctx->first_iteration = 1;
			ctx->segm_ctx->lfb = 0; // should be common for all segmentator's contexts
			if ( lvl2_acc_base_pack(&ctx->base, ctx->msg->meta.port, ctx->msg->meta.is_system, ctx->msg->meta.id) ) {
				msh_finish(CAN_NET_RC_BAD_DATA, ctx);
				return;
			}
		}
		// lfb should be set to 1 in last segmentator iteration
		if (!ctx->segm_ctx->lfb) {
			// NOT last frame:
			can_frame_t* new_frame = (can_frame_t*)malloc(sizeof(can_frame_t));
			int rc = segmentator_num_lfb_async(new_frame, ctx->msg, ctx->segm_ctx);
			if (rc == 2) {
				msh_finish(CAN_NET_RC_TOO_LONG_MSG, ctx);
				return; }
			if (rc == 1) {
				msh_finish(CAN_NET_RC_BAD_DATA, ctx);
				return; }
			if ( lvl2_std_pack(&new_frame->data, ctx->base) ) {
				msh_finish(CAN_NET_RC_BAD_DATA, ctx);
				return; }
			if ( can_frame_pack(new_frame, CAN_NET_LVL2_VER_STD, ctx->msg->meta.hw_addr) ) {
				msh_finish(CAN_NET_RC_BAD_DATA, ctx);
				return; }

			#ifdef CAN_NET_CONFIRMATION
			// register confirmation waiting before sending last frame
			if ( (ctx->msg->meta.is_system != 1) || (ctx->msg->meta.id != SYSMSG_HEARTBEAT_ID) ) {
				// not heartbeat:
				if (ctx->segm_ctx->lfb) {
					MUTEX_L(&confirm_waiters_mutex)
					add_confirm_waiter(ctx->msg, confirmation_tics_g, ctx->callback, ctx->cb_ctx);
					MUTEX_U(&confirm_waiters_mutex)
				}
			}
			#endif
			can_send(new_frame, ctx);
		} else {
			// last frame has already been sent:
			#ifdef CAN_NET_QUEUING
			// start processing new msg if there is any:
			int process_next = 0;
			MUTEX_L(&data_mutex)
			port_descriptor_t* p_p = NULL;
			can_node_descriptor_t* can_node_p = NULL;
			atom_descriptor_t* atom = get_atom_wtrace(ctx->msg->meta.port, ctx->msg->meta.hw_addr, ctx->msg->meta.id, ctx->msg->meta.is_system, &can_node_p, &p_p);
			atom->sending--;
			msgs_queue_t* next_msg = list_pop(atom->msgs_queue);
			if (next_msg != NULL) { // equal to (atom->sending>0) here
				process_next = 1;
			} else {
				// if it has nothing to do - try to delete:
				if (!atom->is_receiving) {
					// here we are sure it's not sending
					list_remove(can_node_p->atoms, atom);
					atom_free(atom);
				}
				// else - (impossible on linux! whole receiving is under data_mutex)
			}
			MUTEX_U(&data_mutex)
			#endif

			msh_finish(CAN_NET_RC_NORM, ctx);
			//<open kostyil>
			#ifdef CAN_NET_QUEUING
			if (process_next)
				msg_send_handler(CAN_DRV_RC_NORM, NULL, next_msg->ctx);

			if(next_msg) free(next_msg);
			//<close kostyil>
			#endif
		}
	} else {
		// something went wrong:
		#ifdef CAN_NET_CONFIRMATION
		if ( (ctx->msg->meta.is_system != 1) || (ctx->msg->meta.id != SYSMSG_HEARTBEAT_ID) ) {
			// not heartbeat:
			if (ctx->segm_ctx->lfb) {
				// err while sending last frame => remove it from confirm_waiters:
				MUTEX_L(&confirm_waiters_mutex)
				confirm_waiter_t* waiter = find_confirm_waiter(ctx->msg->meta.port, ctx->msg->meta.hw_addr, ctx->msg->meta.is_system, ctx->msg->meta.id);
				remove_confirm_waiter(waiter);
				MUTEX_U(&confirm_waiters_mutex)
			}
		}
		#endif
		msh_finish(CAN_NET_RC_DRIVER_ERRS_START + drv_rc, ctx);
	}
}
#undef FINISH

// INTERFACE:
void can_net_start_sending_msg(const msg_lvl2_t* msg, can_net_send_callback_t send_callback, void* cb_ctx) {
	send_msg_context_t* send_ctx = (send_msg_context_t*)malloc(sizeof(send_msg_context_t));
	send_ctx->callback = send_callback;
	send_ctx->cb_ctx = cb_ctx;
	send_ctx->first_iteration = 1;
	// copy msg to context:
	send_ctx->msg = msg_lvl2_make_copy(msg);

	#ifdef CAN_NET_QUEUING
	MUTEX_L(&data_mutex)
	// if atom is sending something at the moment - place given msg in queue:
	can_node_descriptor_t* cn_p = NULL;
	port_descriptor_t* p_p = NULL;
	atom_descriptor_t* atom = get_atom_wtrace(msg->meta.port, msg->meta.hw_addr, msg->meta.id, msg->meta.is_system, &cn_p, &p_p);
	atom->sending++;
	if (atom->sending > 1) {
		// there is already at least one msg - place in queue:
		msgs_queue_t* msg_to_send = (msgs_queue_t*)malloc(sizeof(msgs_queue_t));
		msg_to_send->ctx = send_ctx;
		list_add(atom->msgs_queue, msg_to_send);
		MUTEX_U(&data_mutex)
		return;
	}
	MUTEX_U(&data_mutex)
	#endif

	msg_send_handler(CAN_DRV_RC_NORM, NULL, send_ctx);
}

/*----------------------------------------------IN---------------------------------------------*/

// return 1 if last frame, 0 if not last
int accept_frame(const lvl_segm_fnl_t* segment, atom_descriptor_t* atom, const uint8_t hwa, const uint8_t port) {
	// add received data to buffer:
	frame_data_t_list* saved_frame_data = (frame_data_t_list*)malloc(sizeof(frame_data_t_list));
	saved_frame_data->it = segment->data;
	list_add(atom->data, saved_frame_data);

	if (segment->lfb) {
		msg_lvl2_t_list* msg = (msg_lvl2_t_list*)malloc(sizeof(msg_lvl2_t_list));
		msg->it.meta.hw_addr = hwa;
		msg->it.meta.id = atom->id;
		msg->it.meta.is_system = atom->is_system;
		msg->it.meta.port = port;

		// calculate total size of data: (we know how msg was constructed, so ...)
		msg->it.data.len = (atom->last_frame_num + 1) * CAN_NET_SEGM_FNL_DATALEN_B + segment->data.len;
		msg->it.data.itself = (uint8_t*)malloc(msg->it.data.len);

		// concatenate all received data:
		frame_data_t_list* curr_frame = list_head(atom->data);
		int curr_pos = 0;
		while(curr_frame != NULL) {
			memcpy(&msg->it.data.itself[curr_pos], curr_frame->it.itself, curr_frame->it.len);
			curr_pos += curr_frame->it.len;
			curr_frame = list_item_next(curr_frame);
		}

		// send confirmation of msg receiving:
		int do_not_call_cb = 0;
		#ifdef CAN_NET_CONFIRMATION
		if ( (msg->it.meta.is_system != 1) || (msg->it.meta.id != SYSMSG_HEARTBEAT_ID) ) {
			// not heartbeat:
			uint16_t base = 0;
			can_frame_t* new_frame = (can_frame_t*)malloc(sizeof(can_frame_t));
			if ( lvl2_acc_base_pack(&base, port, msg->it.meta.is_system, msg->it.meta.id) ) {
				//FAILed to send confirmation:
				do_not_call_cb = 1;
			} else if ( lvl2_std_confirm_pack(&new_frame->data, base) ) {
				do_not_call_cb = 1;
			} else {
				if ( can_frame_pack(new_frame, CAN_NET_LVL2_VER_STD_CONFIRM, msg->it.meta.hw_addr) ) {
					do_not_call_cb = 1;
				} else {
					can_send(new_frame, NULL);
				}
			}
		}
		#endif

		if (!do_not_call_cb) {
			MUTEX_L(&recv_callbacks_mutex)
			MUTEX_U(&data_mutex) // - kostyil
			can_net_recv_cb_record_t_plist* curr = list_head(recv_callbacks);
			while(curr != NULL) {
				if ( check_base_range(curr->it_p->check, port, msg->it.meta.is_system, msg->it.meta.id) )
					curr->it_p->callback(&msg->it, curr->it_p->cb_ctx);
				curr = list_item_next(curr);
			}
			MUTEX_L(&data_mutex)
			MUTEX_U(&recv_callbacks_mutex)
		}
		free(msg);
		return 1;
	} else {
		atom->last_frame_num = segment->num;
		return 0;
	}
}

void frame_recv_handler(can_frame_t *frame) {
	#ifdef CAN_NET_LINUX
	can_net_thread_id = (pid_t)syscall(__NR_gettid); //gettid()
	#endif

	lvl2_std_t lvl2_std;
	port_descriptor_t* port_p = NULL;
	can_node_descriptor_t* can_node = NULL;
	atom_descriptor_t* atom = NULL;

	#ifdef CAN_NET_CONFIRMATION
	lvl2_std_confirm_t lvl2_conf;
	#endif

	frame_head_t frame_head = frame_head_unpack(frame->inf.id, frame->inf.ide);
	if (frame->inf.err) {
		//TODO: PANICPANICPANICPANICPANICPANICPANICPANIC;
	}
	//if (frame->inf.rtr) do WTF is rtr;
	//if (frame->inf.eff) do nothing;
	MUTEX_L(&data_mutex)
	switch (frame_head.version) {
		case CAN_NET_LVL2_VER_STD:
			if (frame->data.len < CAN_NET_LVL2_STD_HEADLEN_B) {
				//add_error(CAN_NET_RC_TOO_SHORT_STD_FRAME);
				break;
			}
			lvl2_std = lvl2_std_unpack(&frame->data);
			atom = get_atom_wtrace(lvl2_std.base.port, frame_head.hw_addr, lvl2_std.base.id, lvl2_std.base.smb, &can_node, &port_p);
			// check rules for frame number:
			if (lvl2_std.segm.num == 0) {
				#ifdef CAN_NET_QUEUING
				atom->is_receiving = 1;
				#endif
				if (list_head(atom->data) != NULL) {
					// just a warning, start to read new msg:
					//add_error(CAN_NET_RC_NEW_MSG_BEFORE_END_OF_OLD);
					atom_clear_on_read(atom);
					#ifdef CAN_NET_QUEUING
					atom->is_receiving = 1;
					#endif
				}
			} else if ((lvl2_std.segm.num - atom->last_frame_num) != 1) {
				//add_error(CAN_NET_RC_STD_WRONG_FRAMES_ORDER);
				list_remove(can_node->atoms, atom);
				atom_free(atom);
				break;
			}

			if ( accept_frame(&lvl2_std.segm, atom, can_node->hw_addr, port_p->code) ) {
				int delete_atom = 1;

				#ifdef CAN_NET_QUEUING
				// here we are sure atom is not reading
				if (atom->sending == 0)
					delete_atom = 0;
				atom->is_receiving = 0;
				#endif

				if (delete_atom) {
					list_remove(can_node->atoms, atom);
					atom_free(atom);
				} else {
					// at least, clean it for next read
					atom_clear_on_read(atom);
				}
			}
			break;

		#ifdef CAN_NET_CONFIRMATION
		case CAN_NET_LVL2_VER_STD_CONFIRM:
			if (frame->data.len < CAN_NET_LVL2_STD_CONFIRM_SIZE_CONST) {
				//add_error(CAN_NET_RC_TOO_SHORT_STD_CONFIRM);
				break;
			}
			lvl2_conf = lvl2_std_confirm_unpack(&frame->data);

			MUTEX_L(&confirm_waiters_mutex)
			confirm_waiter_t* waiter = find_confirm_waiter(lvl2_conf.port, frame_head.hw_addr, lvl2_conf.smb, lvl2_conf.id);
			if (waiter != NULL) {
				// valid confirmation - call callback
				call_scb(waiter->callback, CAN_NET_RC_NORM, waiter->msg, waiter->cb_ctx);
				remove_confirm_waiter(waiter);
			}
			// else - confirmation to nonexistent waiter
			MUTEX_U(&confirm_waiters_mutex)

			break;
		#endif

		default:
			//add_error(CAN_NET_RC_UNKNOWN_PROTOCOL);
			break;
	}
	MUTEX_U(&data_mutex)
}
