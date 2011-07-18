// created 19.05.2011
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


#ifndef CAN_NET_CORE_H_
#define CAN_NET_CORE_H_

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
