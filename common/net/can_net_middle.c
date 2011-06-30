// 19.05.2011
// 03.06.2011
// to be changed later:
// MITL

#include "can_net_middle.h"

/* -------------------------------------INTERFACE_STRUCTS---------------------------------------*/

//and small bonus
inline void call_scb(can_net_send_callback_t send_cb, const int rc, msg_lvl2_t* msg) {
	if(send_cb != NULL)
	    send_cb(rc, msg);

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

inline void add_confirm_waiter(msg_lvl2_t* msg, const uint32_t tics, const can_net_send_callback_t cb) {
	confirm_waiter_t* cw = (confirm_waiter_t*)malloc(sizeof(confirm_waiter_t));
	cw->msg = msg;
	cw->tics_left = tics;
	cw->callback = cb;
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

void check_confirm_waiters() {
	confirm_waiter_t* curr_ = list_head(confirm_waiters_g);
	while(curr_ != NULL) {
		--curr_->tics_left;
		if (curr_->tics_left <= 0) {
			// remove waiter if time ran out
			call_scb(curr_->callback, CAN_NET_RC_CONF_TIMEOUT, curr_->msg);
			remove_confirm_waiter(curr_);
		}
		curr_ = list_item_next(curr_);
	}
}

#endif


