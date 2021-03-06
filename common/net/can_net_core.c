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


#include "can_net_core.h"

/* -------------------------------------BICYCLES------------------------------------------------*/

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
/*
typedef struct {
	void* next;
	void* it_p;
} abstract_plist_t;

// Yet another bicycle:
void list_free_all_p(list_t list) {
	abstract_plist_t item = list_head(list);
	void* next_item;
	while(item != NULL) {
		next_item = list_item_next(item);
		free(item->it_p);
		free(item);
		item = next_item;
	}
	// make head NULL:
	*list = NULL;
}
*/
/* ------------------------------------------CORE_DATA------------------------------------------*/

LIST(ports_g);

/* ------------------------------------------CORE_FUNCS-----------------------------------------*/

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
	port_descriptor_t* curr_ = list_head(ports_g);
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
		list_add(ports_g, res);
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

inline atom_descriptor_t* get_atom_wtrace(const uint8_t port, const uint32_t hwa, const uint16_t id, const uint8_t smb,
									can_node_descriptor_t** can_node_p, port_descriptor_t** port_p) {
	*port_p = get_port(port);
	*can_node_p = get_can_node(hwa, *port_p);
	return get_atom(id, smb, *can_node_p);
}

