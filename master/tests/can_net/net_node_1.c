#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include "../common.h"

int main(void) {
// no recv callbacks for sender
//	can_net_recv_cb_record_t* callback_p = (can_net_recv_cb_record_t*)malloc(sizeof(can_net_recv_cb_record_t));
//	callback_p->cb_ctx = NULL;
//	callback_p->callback = NULL;
//	can_net_add_callback(recv_callbacks);

	uint32_t msg_confirm_tics = 1000; // 1000ms=1s
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

// sender:
	uint8_t my_port = 1;
	uint32_t hwa = 15;
	uint16_t id = 18;
	int rc;
	int i;
	for (i=0; i<10; i++) {
		rc = test_send_msg(hwa, id, 0, my_port, TEST_SHORT, test_send_callback);
		printf("TEST_SHORT sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id, 0, my_port, TEST_LONG, test_send_callback);
		printf("TEST_LONG sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id, 0, my_port, TEST_LONG, test_send_callback);
		printf("TEST_LONG 2 sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id+1, 1, my_port, TEST_LONG, test_send_callback);
		printf("TEST_LONG 3 sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id+2, 0, my_port, TEST_LONG, test_send_callback);
		printf("TEST_LONG 4 sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id+3, 1, my_port+1, TEST_LONG, test_send_callback);
		printf("TEST_LONG 5 sent: rc= %d\n\n", rc);

		rc = test_send_msg(hwa, id+4, 0, my_port, TEST_LONG, test_send_callback);
		printf("TEST_LONG 6 sent: rc= %d\n\n", rc);

		printf("----%d----\n\n", i);
	//	usleep(1000);
	}

#ifdef TEST_ERRORS
	rc = test_send_msg(hwa, id, 0, my_port, TEST_ERR_UNKNOWN_PROTOCOL, test_send_callback);
	printf("ERR_UNKNOWN_PROTOCOL sent: rc= %d\n\n", rc);

//	rc = test_send_msg(hwa, id, 0, my_port, TEST_ERR_MSG_FROM_UNKNOWN_NODE, test_send_callback);
//	printf("ERR_MSG_FROM_UNKNOWN_NODE sent: rc= %d\n\n", rc);

	rc = test_send_msg(hwa, id, 0, my_port, TEST_ERR_NEW_MSG_BEFORE_END_OF_OLD, test_send_callback);
	printf("ERR_NEW_MSG_BEFORE_END_OF_OLD sent: rc= %d\n\n", rc);

	rc = test_send_msg(hwa, id, 0, my_port, TEST_ERR_WRONG_FRAMES_ORDER, test_send_callback);
	printf("WRONG_FRAMES_ORDER sent: rc= %d\n\n", rc);
#endif
/*
	srand (time(NULL));
	rc = test_send_msg(rand()%32000, rand()%250, rand()%250, rand()%100, TEST_RANDOM_TRASH, test_send_callback);
	printf("TEST_RANDOM_TRASH sent: rc= %d\n\n", rc);
*/
/* check response:
	rc += test_msg(hwa, id, 1, my_port, TEST_SUCCESFUL_TEST);
*/

//	can_net_server_in_stop();
	printf("sleeping\n");
	sleep(100500);
	printf("\n the end\n");

	return 0;
}
//usleep(sleep_time_ms * 1000);
