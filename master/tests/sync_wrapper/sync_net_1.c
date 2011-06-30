/*
 * can_net_sync_wrapper.h
 *
 *  Created on: Jun 27, 2011
 *      Author: Gennady.Kalashnikov
 */

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
	uint32_t send_frame_timeout_us = 1000; // = 1ms
	uint32_t confirmation_tics = 1000; // *1ms = 1s
	int msgs_limit = 10;
	int timeout_cycles = -1;
	int rci = can_net_sync_init(port, msgs_limit, timeout_cycles, send_frame_timeout_us, confirmation_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

// sender:
	int rc = 0;
	int i;

	msg_lvl2_t* msg = (msg_lvl2_t*)malloc(sizeof(msg_lvl2_t));
	msg->meta.hw_addr = 15;
	msg->meta.id = 18;
	msg->meta.is_system = 0;
	msg->meta.port = port;

	for (i=0; i<10; i++) {
		int_msg_data(msg, test_short_msg_int);
		rc = can_net_sync_send(msg);
		printf("TEST_SHORT sent: rc= %d\n\n", rc);

		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG sent: rc= %d\n\n", rc);

		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG 2 sent: rc= %d\n\n", rc);

		msg->meta.id = 19;
		msg->meta.is_system = 1;
		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG 3 sent: rc= %d\n\n", rc);

		msg->meta.id = 20;
		msg->meta.is_system = 1;
		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG 4 sent: rc= %d\n\n", rc);

		msg->meta.id = 21;
		msg->meta.is_system = 1;
		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG 5 sent: rc= %d\n\n", rc);

		msg->meta.id = 22;
		msg->meta.is_system = 0;
		set_msg_data_long(msg);
		rc = can_net_sync_send(msg);
		printf("TEST_LONG 6 sent: rc= %d\n\n", rc);

		printf("----%d----\n\n", i);

}

	printf("sleeping\n");
	sleep(100500);
	printf("\n the end\n");

	return 0;
}
//usleep(sleep_time_ms * 1000);
