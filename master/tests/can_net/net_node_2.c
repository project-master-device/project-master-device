#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include "../common.h"

/*
void try_receive_rand_mgs(int my_port) {
	int i;
	for(i=0; i<10; i++) {
		msg_lvl2_t* msg_in = can_net_receive_msg(my_port);
		if (msg_in != NULL) {
			if (test_cmp_msg(msg_in, 777, 777, 1, TEST_RANDOM_TRASH)) {
				printf("\n BINGO! YOU WON 1000000$ \n");
			} else {
				printf("received random message\n");
				test_print_msg(msg_in);
			}
			break;
		}
		int err = can_net_server_in_get_error(my_port);
		if (err != CAN_NET_ERR_NO_NEW_ERRORS) {
			printf("\n----RAND------- %d error\n", err);
			break;
		}
		usleep(sleep_time_us);
	}
}
*/

int main(void) {
	can_net_recv_cb_record_t* cbr_p = (can_net_recv_cb_record_t*)malloc(sizeof(can_net_recv_cb_record_t));
	cbr_p->callback = test_recv_callback;
	cbr_p->cb_ctx = NULL;
	cbr_p->check.port_min = 0;
	cbr_p->check.port_max = -1;
	cbr_p->check.id_min = 0;
	cbr_p->check.id_max = -1;
	cbr_p->check.smb_min = 0;
	cbr_p->check.smb_max = 1;
	can_net_add_callback(cbr_p);

	uint32_t msg_confirm_tics = 1000; // 1000ms=1s
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

//	uint32_t hwa = 15;
//	uint16_t id = 18;
//	int rc = 0;
/*
// receive expected short+long messages:
	rc += test_msg(hwa, id, 0, my_port, TEST_SHORT, NULL);

	rc += test_msg(hwa, id, 0, my_port, TEST_LONG, NULL);
*/
#ifdef TEST_ERRORS
// receive expected errors:
	rc += test_err(my_port, CAN_NET_ERR_UNKNOWN_PROTOCOL, NULL);

//	rc += test_err(my_port, CAN_NET_ERR_MSG_FROM_UNKNOWN_NODE, NULL);

	rc += test_err(my_port, CAN_NET_ERR_NEW_MSG_BEFORE_END_OF_OLD, NULL);

	rc += test_err(my_port, CAN_NET_ERR_STD_WRONG_FRAMES_ORDER, NULL);
	rc += test_msg(hwa, id, 0, my_port, TEST_SHORT, NULL);
#endif

//	try_receive_rand_mgs(my_port);

/*
	if (!rc) {
		printf("SUCCESFUL_TEST sending...\n");
		int rcs = test_send_msg(hwa, id, 1, my_port, TEST_SUCCESFUL_TEST);
		printf("SUCCESFUL_TEST sent: rc= %d\n", rcs);
	}
*/

//	can_net_server_in_stop();
	printf("\nsleeping\n");
	sleep(100500);
	printf("\nthe end\n");

	return 0;
}
