/*
 * can_net_structs.h
 *
 *  Created on: May 19, 2011
 *      Author: mgaldoru
 */

#ifndef CAN_NET_STRUCTS_H_
#define CAN_NET_STRUCTS_H_

LIST(ports);
#ifdef CAN_NET_CONFIRMATION
LIST(confirm_waiters);
#endif
// bicycle, 1=TRUE, 0=FALSE
int check_base_range(can_net_base_range_t ranges, uint8_t port, uint8_t smb, uint16_t id) {
	if ( (port > ranges.port_max) || (port < ranges.port_min) )
		return 0;
	if ( (id > ranges.id_max) || (id < ranges.id_min) )
		return 0;
	if ( (smb > ranges.smb_max) || (smb < ranges.smb_min) )
		return 0;
	return 1;
}

// Yet another bicycle:
void list_free_all(list_t list) {
	void* item = list_head(list);
	void* next_item;
	while(item != NULL) {
		next_item = list_item_next(item);
		free(item);
		item = next_item;
	}
	// make head NULL:
	*list = NULL;
}

typedef struct {
	void* next; // to use in list
	LIST_STRUCT(can_nodes);
	uint8_t code;
} port_descriptor_t;

#ifdef CAN_NET_CONFIRMATION
typedef struct {
	void* next; // to use in list
	uint8_t port;
	msg_lvl2_t* msg;
	int tics_left; // num of timer_tics else to wait
	can_net_send_callback_t callback;
} confirm_waiter_t;
#endif

typedef struct {
	void* next; // to use in list
	uint8_t is_system;
	uint16_t id;
	int16_t last_frame_num; // keep it signed! important kostyil!
	LIST_STRUCT(data);
#ifdef CAN_NET_QUEUING
	// to allow queuing of msgs to one atom:
	LIST_STRUCT(msgs_queue);
	uint8_t sending;
	uint8_t is_receiving;
#endif
} atom_descriptor_t;

typedef struct {
	void* next; // to use in list
	uint32_t hw_addr;
	LIST_STRUCT(atoms);
} can_node_descriptor_t;

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
	msg_lvl2_t it;
} msg_lvl2_t_list;

typedef struct {
	void* next; // to use in list
	can_frame_t it;
} can_frame_t_list;

void atom_clear_on_read(atom_descriptor_t* atom) {
	atom->id = 0;
	atom->is_system = 0;
	atom->last_frame_num = -1;
	list_free_all(atom->data);
}

void atom_free(atom_descriptor_t* atom) {
	list_free_all(atom->data);
	#ifdef CAN_NET_QUEUING
	list_free_all(atom->msgs_queue);
	#endif
	free(atom);
}

/*
void can_node_clear(can_node_descriptor_t* can_node) {
	can_node->meta.id = 0;
	can_node->meta.is_system = 0;
	can_node->last_frame_num = -1;
	list_free_all(can_node->data);
}
*/

// FIND functions: returns NULL if found didn't find

// TODO: always use 'port_descriptor_t* port_p', not 'uint8_t port' in output funcs
port_descriptor_t* find_port(uint8_t port) {
	port_descriptor_t* curr_ = list_head(ports);
	while(curr_ != NULL) {
		if (curr_->code == port) {
			return curr_;
		}
		curr_ = list_item_next(curr_);
	}
	return NULL;
}

can_node_descriptor_t* find_can_node(const uint32_t hwa, const port_descriptor_t* port_p) {
	can_node_descriptor_t* curr_ = list_head(port_p->can_nodes);
	while(curr_ != NULL) {
		if (curr_->hw_addr == hwa) {
			return curr_;
		}
		curr_ = list_item_next(curr_);
	}
	return NULL;
}

atom_descriptor_t* find_atom(const uint16_t id, const uint8_t is_system, const can_node_descriptor_t*  can_node_p) {
	atom_descriptor_t* curr_ = list_head(can_node_p->atoms);
	while(curr_ != NULL) {
		if (curr_->id == id) {
			if (curr_->is_system == is_system)
				return curr_;
		}
		curr_ = list_item_next(curr_);
	}
	return NULL;
}

// GET functions: returns new element if found didn't find

// TODO: always use 'port_descriptor_t* port_p', not 'uint8_t port' in output funcs
port_descriptor_t* get_port(uint8_t port) {
	port_descriptor_t* res = find_port(port);
	if (res == NULL) {
		// no such port => make new:
		res = (port_descriptor_t*)malloc(sizeof(port_descriptor_t));
		list_add(ports, res);
		LIST_STRUCT_INIT(res, can_nodes);
		res->code = port;
	}
	return res;
}

can_node_descriptor_t* get_can_node(const uint32_t hwa, const port_descriptor_t* port_p) {
	can_node_descriptor_t* res = find_can_node(hwa, port_p);
	if (res == NULL) {
		res = (can_node_descriptor_t*)malloc(sizeof(can_node_descriptor_t));
		list_add(port_p->can_nodes, res);
		LIST_STRUCT_INIT(res, atoms);
		res->hw_addr = hwa;
	}
	return res;
}

atom_descriptor_t* get_atom(const uint16_t id, const uint8_t is_system, const can_node_descriptor_t* can_node_p) {
	atom_descriptor_t* res = find_atom(id, is_system, can_node_p);
	if (res == NULL) {
		res = (atom_descriptor_t*)malloc(sizeof(atom_descriptor_t));
		list_add(can_node_p->atoms, res);
		LIST_STRUCT_INIT(res, data);
		atom_clear_on_read(res);
		res->id = id;
		res->is_system = is_system;
		#ifdef CAN_NET_QUEUING
		LIST_STRUCT_INIT(res, msgs_queue);
		res->is_receiving = 0;
		res->sending = 0;
		#endif
	}
	return res;
}

atom_descriptor_t* get_atom_wtrace(const uint8_t port, const uint32_t hwa, const uint8_t smb, const uint16_t id,
									can_node_descriptor_t** can_node_p, port_descriptor_t** port_p) {
	*port_p = get_port(port);
	*can_node_p = get_can_node(hwa, *port_p);
	atom_descriptor_t* atom_p = get_atom(id, smb, *can_node_p);
	return atom_p;
}

// TODO: do smth with it:
#ifdef CAN_NET_CONFIRMATION
confirm_waiter_t* find_conf_waiter(uint8_t port, uint32_t hwa, uint8_t smb, uint16_t id) {
	confirm_waiter_t* curr_ = list_head(confirm_waiters);
	while(curr_ != NULL) {
		if ((curr_->port == port) && (curr_->msg->meta.hw_addr == hwa) &&
			(curr_->msg->meta.is_system == smb) && (curr_->msg->meta.id == id)) {
			return curr_;
		}
		curr_ = list_item_next(curr_);
	}
	return NULL;
}
#endif

#endif /* CAN_NET_STRUCTS_H_ */
