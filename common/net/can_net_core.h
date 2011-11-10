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


#ifndef CAN_NET_CORE_H_
#define CAN_NET_CORE_H_

//TODO:CANNET_REFACTORING rename to can_net_internal_structs

#include <stdlib.h>
#include "can_protocols.h"
#include "../lib/list.h"
#include "can_drv.h"

/* defines:
		CAN_NET_QUEUING			- use queuing (allows fast msgs sending in right order)
*/
#define CAN_NET_QUEUING

/* -------------------------------------BICYCLES------------------------------------------------*/

// Yet another bicycle:
void list_free_all(list_t list);

/* ------------------------------------------CORE_STRUCTS---------------------------------------*/

typedef struct {
	void* next; // to use in list
	LIST_STRUCT(can_nodes);
	uint8_t code;
} port_descriptor_t;

typedef struct {
	void* next; // to use in list
	uint32_t hw_addr;
	LIST_STRUCT(atoms);
} can_node_descriptor_t;

typedef struct {
	void* next; // to use in list
	uint8_t is_system;
	uint16_t id;
	int16_t last_frame_num; // keep it signed! important kostyil!
	LIST_STRUCT(data);
	#ifdef CAN_NET_QUEUING
	LIST_STRUCT(msgs_queue);
	uint8_t sending;
	uint8_t is_receiving;
	#endif
} atom_descriptor_t;

/* ------------------------------------------CORE_FUNCS-----------------------------------------*/

void atom_clear_on_read(atom_descriptor_t* atom);

void atom_free(atom_descriptor_t* atom);


// FIND functions: returns NULL if found didn't find

port_descriptor_t* find_port(uint8_t port);

can_node_descriptor_t* find_can_node(const uint32_t hwa, const port_descriptor_t* port_p);

atom_descriptor_t* find_atom(const uint16_t id, const uint8_t is_system, const can_node_descriptor_t* can_node_p);


// GET functions: returns new element if found didn't find

port_descriptor_t* get_port(uint8_t port);

can_node_descriptor_t* get_can_node(const uint32_t hwa, const port_descriptor_t* port_p);

atom_descriptor_t* get_atom(const uint16_t id, const uint8_t is_system, const can_node_descriptor_t* can_node_p);

inline atom_descriptor_t* get_atom_wtrace(const uint8_t port, const uint32_t hwa, const uint16_t id, const uint8_t smb,
									can_node_descriptor_t** can_node_p, port_descriptor_t** port_p);


#endif /* CAN_NET_CORE_H_ */
