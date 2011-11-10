#include "common.h"

unsigned sleep_time_us = 10 * 1000; // 10ms
unsigned sleep_time_us_inter = 100 * 1000; // 100ms

uint32_t test_short_msg_int = 1971960389;

char* test_long_msg = "1234567890abcdef";
uint32_t test_long_msg_len = 16;//111;

#ifdef TEST_ERRORS
uint32_t test_unknown_hwa = 42;
uint8_t test_unknown_version = 26;
#endif

uint32_t test_succesful_test_msg = 123456789;


void int_msg_data(msg_lvl2_t* msg, uint32_t data) {
	msg->data.len = 4;
	msg->data.itself = (uint8_t*)malloc(msg->data.len);
	memcpy(msg->data.itself, (char*)&data, msg->data.len);
}

void set_msg_data_long(msg_lvl2_t* msg) {
	msg->data.len = test_long_msg_len;
	msg->data.itself = (uint8_t*)malloc(msg->data.len);
	memcpy(msg->data.itself, test_long_msg, msg->data.len);
}

void test_print_msg(const msg_lvl2_t* msg) {
	printf("	msg->meta.hw_addr=%d\n", msg->meta.hw_addr);
	printf("	msg->meta.port=%d\n", msg->meta.port);
	printf("	msg->meta.id=%d\n", msg->meta.id);
	printf("	msg->meta.is_system=%d\n", msg->meta.is_system);
	printf("	msg->data.len=%d\n", msg->data.len);
	if (msg->data.len <= 5)
		printf("	msg->data.itself=%d\n", *((uint32_t*)msg->data.itself));
	else
		printf("	msg->data.itself=%.*s\n", msg->data.len, msg->data.itself);
}

int test_send_msg(uint32_t hwa, uint16_t id, uint8_t smb, uint8_t port, int msg_type, can_net_send_callback_t cb) {
	msg_lvl2_t msg;
	msg.meta.hw_addr = hwa;
	msg.meta.id = id;
	msg.meta.is_system = smb;
	msg.meta.port = port;
	//	can_frame_data_t frame_data;
	int rc = 0;
	switch (msg_type) {
		case TEST_SHORT:
			int_msg_data(&msg, test_short_msg_int);
			break;

		case TEST_LONG:
			set_msg_data_long(&msg);
			break;
#ifdef TEST_ERRORS
		case TEST_ERR_UNKNOWN_PROTOCOL:
			int_msg_data(&msg, test_short_msg_int);
			pack_can_data_std(&frame_data, port, msg.data.itself, msg.data.len, CAN_NET_LVL2_STD_MAX_NUM, 1, &msg.meta);
			rc = send_frame(test_unknown_version, frame_data, msg.meta.hw_addr);
			free(msg.data.itself);
			return rc;

		case TEST_ERR_MSG_FROM_UNKNOWN_NODE:
			int_msg_data(&msg, test_short_msg_int);
			msg.meta.hw_addr = test_unknown_hwa;
			pack_can_data_std(&frame_data, port, msg.data.itself, msg.data.len, CAN_NET_LVL2_STD_MAX_NUM, 1, &msg.meta);
			rc = send_frame(CAN_NET_LVL2_VERSION_STD, frame_data, msg.meta.hw_addr);
			free(msg.data.itself);
			return rc;

		case TEST_ERR_NEW_MSG_BEFORE_END_OF_OLD:
			for (i=0; i<2; i++) {
				int_msg_data(&msg, test_short_msg_int);
				pack_can_data_std(&frame_data, port, msg.data.itself, msg.data.len, i, 0, &msg.meta);
				rc = send_frame(CAN_NET_LVL2_VERSION_STD, frame_data, msg.meta.hw_addr);
			}
			int_msg_data(&msg, test_short_msg_int);
			break;

		case TEST_ERR_WRONG_FRAMES_ORDER:
			for (i=0; i<3; i++) {
				int_msg_data(&msg, test_short_msg_int);
				pack_can_data_std(&frame_data, port, msg.data.itself, msg.data.len, i, 0, &msg.meta);
				rc = send_frame(CAN_NET_LVL2_VERSION_STD, frame_data, msg.meta.hw_addr);
			}
			int_msg_data(&msg, test_short_msg_int);
			pack_can_data_std(&frame_data, port, msg.data.itself, msg.data.len, 2, 0, &msg.meta);
			rc = send_frame(CAN_NET_LVL2_VERSION_STD, frame_data, msg.meta.hw_addr);
			free(msg.data.itself);
			return rc;
#endif
		case TEST_RANDOM_TRASH:
			msg.data.len = 8;
			msg.data.itself = (uint8_t*)malloc(msg.data.len);
			test_print_msg(&msg);
			break;

		case TEST_SUCCESFUL_TEST:
			int_msg_data(&msg, test_succesful_test_msg);
			break;
	}

	//rc =
	can_net_start_sending_msg(&msg, cb, NULL);
	free(msg.data.itself);
	return rc;
}

int test_cmp_msg(msg_lvl2_t* msg, uint32_t hwa, uint16_t id, uint8_t smb, int msg_type) {
	if (msg->meta.hw_addr != hwa) return 0;
	if (msg->meta.id != id) return 0;
	if (msg->meta.is_system != smb) return 0;
	switch (msg_type) {
		case TEST_SHORT:
			if (msg->data.len != 4) return 0;
			if ( *((uint32_t*)msg->data.itself) != test_short_msg_int) return 0;
			break;
		case TEST_LONG:
			if (msg->data.len != test_long_msg_len) return 0;
			if ( strncmp((char*)msg->data.itself, test_long_msg, msg->data.len) ) return 0;
			break;
		case TEST_RANDOM_TRASH:
			//any data is fine
			break;
		case TEST_SUCCESFUL_TEST:
			if (msg->data.len != 4) return 0;
			if ( *((uint32_t*)msg->data.itself) != test_succesful_test_msg) return 0;
			break;
		default:
			return 0;
			break;
	}
	return 1;
}

int test_msg(uint32_t hwa, uint8_t id, uint8_t smb, int my_port, int msg_type, msg_lvl2_t* msg_in) {
	char name[100];
	switch (msg_type) {
		case TEST_SHORT:
			strcpy(name, "SHORT");
			break;
		case TEST_LONG:
			strcpy(name, "LONG");
			break;
		case TEST_SUCCESFUL_TEST:
			strcpy(name, "SUCCESFUL_TEST");
			break;
	}
	if (test_cmp_msg(msg_in, hwa, id, smb, msg_type)) {
		printf("\n----SUCCESS---- %s msg received!\n", name);
		return 0;
	} else {
		printf("\n----FAIL------- Something wrong in %s message\n", name);
		test_print_msg(msg_in);
		return 1;
	}
}

void test_send_callback(const int rc, msg_lvl2_t* msg, void* context) {
	printf("\n send_callback: rc=%d\n", rc);
	test_print_msg(msg);
}
void test_recv_callback(const msg_lvl2_t* msg, void* context) {
	printf("\n receive_callback: \n");
	test_print_msg(msg);
}

#ifdef TEST_ERRORS
int test_err(int my_port, int err_type) {
	char name[100];
	switch (err_type) {
		case CAN_NET_ERR_TAIL_TO_NULL_CANNODE:
			strcpy(name, "TAIL_TO_NULL_CANNODE");
			break;
		case CAN_NET_ERR_UNKNOWN_PROTOCOL:
			strcpy(name, "UNKNOWN_PROTOCOL");
			break;
		case CAN_NET_ERR_NEW_MSG_BEFORE_END_OF_OLD:
			strcpy(name, "NEW_MSG_BEFORE_END_OF_OLD");
			break;
		case CAN_NET_ERR_STD_WRONG_FRAMES_ORDER:
			strcpy(name, "WRONG_FRAMES_ORDER");
			break;
	}
	int err;
	while(1) {
		err = can_net_server_in_get_error(my_port);
		if (err != CAN_NET_ERR_NO_NEW_ERRORS) {
			if (err == err_type) {
				printf("\n----SUCCESS---- %s error\n", name);
				return 0;
			} else {
				printf("\n----FAIL------- not %s error (err=%d)\n", name, err);
				return 1;
			}
		}
		usleep(sleep_time_us);
	}
}
#endif

void test_print_time() {
	char buffer[30];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&tv.tv_sec));
	printf("%s%ld\n",buffer,tv.tv_usec);
}
