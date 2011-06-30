#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../../../common/net/can_net.h"
#include "../../../../common/pmd_net/pmd_reader.h"

void send() {
    msg_lvl2_t msg;
    uint8_t msg_buf[64];
    msg.meta.id = 5;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = 0;
    msg.data.itself = msg_buf;
    msg.data.len = 1;

    pmd_reader_data_t d;

    d.operation = PMD_READER_GREEN_LED_ON;
    pmd_reader_write_data(msg.data, &d); 
    can_net_start_sending_msg(&msg, NULL);

    sleep(1);

    d.operation = PMD_READER_BEEP_ON;
    pmd_reader_write_data(msg.data, &d); 
    can_net_start_sending_msg(&msg, NULL);

    sleep(1);

    d.operation = PMD_READER_BEEP_OFF;
    pmd_reader_write_data(msg.data, &d); 
    can_net_start_sending_msg(&msg, NULL);

    sleep(1);

    d.operation = PMD_READER_GREEN_LED_OFF;
    pmd_reader_write_data(msg.data, &d); 
    can_net_start_sending_msg(&msg, NULL);
}

int main(int argc, char * argv[]) {
	can_net_recv_callbacks_arr_t recv_callbacks;
	recv_callbacks.len = 0;
	recv_callbacks.records = NULL;
	can_net_add_callbacks(recv_callbacks);

    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");


    send();

	printf("sleeping\n");
	sleep(100500);
	printf("the end\n");

	return 0;
}
