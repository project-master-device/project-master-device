#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#define CAN_NET_LINUX
#include "../../../../../net/trunk/can_net_core.h"

int main(void) {
	printf("point 0n\n");

	can_node_descriptor_t* can_node_p = NULL;
	port_descriptor_t* port_p = NULL;
	atom_descriptor_t* a = get_atom_wtrace(1, 10, 0, 11, &can_node_p, &port_p);
	printf("point 1n\n");
	a = NULL;

//	confirm_waiter_t* cw = find_conf_waiter(2, 13, 0, 11);
//	printf("point 2n\n");
//	cw = NULL;

	return 0;
}

//
