/*
#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_LINUX
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION
*/

#include "../common.h"
#include "../../CAN_NET_fronts/can_net_sync_wrapper.h"

int main(void) {
	uint8_t port = 1;
	int sync_timeout = 1500; // *1000us = 1.5s
	uint32_t send_frame_timeout_us = 500000; // *1us = 0.5s
	uint32_t net_confirm_timeout = 1000; // *1000us = 1s
	int msgs_limit = 10;
	int rci = can_net_sync_init(port, msgs_limit, sync_timeout, send_frame_timeout_us, net_confirm_timeout);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

// sender:
	int rc = 0;
	int i;

	msg_lvl2_t* msg = NULL;

//	msg->meta.hw_addr = 15;
//	msg->meta.id = 18;
//	msg->meta.is_system = 0;
//	msg->meta.port = port;

	for (i=0; i<7*10; i++) {
		rc = can_net_sync_recv(port, &msg);
		printf("TEST recv: rc= %d\n\n", rc);
		if (!rc) test_print_msg(msg);
	}

	printf("sleeping\n");
	sleep(100500);
	printf("\n the end\n");

	return 0;
}
//usleep(sleep_time_ms * 1000);
