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


#include "can_net_middle.h"

/* -------------------------------------INTERFACE_STRUCTS---------------------------------------*/

// bicycle, 1=TRUE, 0=FALSE
int check_base_range(can_net_base_range_t range, uint8_t port, uint8_t smb, uint16_t id) {
	if ( (port > range.port_max) || (port < range.port_min) )
		return 0;
	if ( (id > range.id_max) || (id < range.id_min) )
		return 0;
	if ( (smb > range.smb_max) || (smb < range.smb_min) )
		return 0;
	return 1;
}

inline int check_base_range_meta(can_net_base_range_t range, msg_metadata_t* meta) {
	return check_base_range(range, meta->port, meta->is_system, meta->id);
}

//and small bonus
inline void call_scb(can_net_send_callback_t send_cb, const int rc, msg_lvl2_t* msg, void* cb_ctx) {
	if(send_cb != NULL) {
        send_cb(rc, msg, cb_ctx);
    }

	free(msg->data.itself);
	free(msg);
}

/* -----------------------------------CONFIRMATION----------------------------------------------*/

#ifdef CAN_NET_CONFIRMATION

LIST(confirm_waiters_g);

inline void remove_confirm_waiter(confirm_waiter_t* waiter) {
	list_remove(confirm_waiters_g, waiter);
	free(waiter);
}

inline void add_confirm_waiter(msg_lvl2_t* msg, const uint32_t tics, const can_net_send_callback_t cb, void* cb_ctx) {
	confirm_waiter_t* cw = (confirm_waiter_t*)malloc(sizeof(confirm_waiter_t));
	cw->msg = msg;
	cw->tics_left = tics;
	cw->callback = cb;
	cw->cb_ctx = cb_ctx;
	list_add(confirm_waiters_g, cw);
}

// TODO: do smth with it:
confirm_waiter_t* find_confirm_waiter(uint8_t port, uint32_t hwa, uint8_t smb, uint16_t id) {
	confirm_waiter_t* curr_ = list_head(confirm_waiters_g);
	while(curr_ != NULL) {
		if ((curr_->msg->meta.port == port) && (curr_->msg->meta.hw_addr == hwa) &&
			(curr_->msg->meta.is_system == smb) && (curr_->msg->meta.id == id)) {
			return curr_;
		}
		curr_ = list_item_next(curr_);
	}
	return NULL;
}

void check_confirm_waiters(void) {
	// TODO: ADD MUTEX
	confirm_waiter_t* next_ = NULL;
	confirm_waiter_t* curr_ = list_head(confirm_waiters_g);
	while(curr_ != NULL) {
		--curr_->tics_left;
		next_ = list_item_next(curr_);
		if (curr_->tics_left <= 0) {
			// remove waiter if time ran out
			call_scb(curr_->callback, CAN_NET_RC_CONF_TIMEOUT, curr_->msg, curr_->cb_ctx);
			remove_confirm_waiter(curr_);
		}
		curr_ = next_ ;
	}
}

#endif


