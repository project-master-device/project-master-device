#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/pmd_reader.h"

void handle() {
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
    //can_net_start_sending_msg(&msg, NULL);

    d.operation = PMD_READER_BEEP_ON;
    pmd_reader_write_data(msg.data, &d); 
    //can_net_start_sending_msg(&msg, NULL);

    //sleep(1);

    d.operation = PMD_READER_BEEP_OFF;
    pmd_reader_write_data(msg.data, &d); 
    //can_net_start_sending_msg(&msg, NULL);

    d.operation = PMD_READER_GREEN_LED_OFF;
    pmd_reader_write_data(msg.data, &d); 
    //can_net_start_sending_msg(&msg, NULL);
}

void foo(pmd_reader_data_t * reader_data) {
    uint32_t code = (reader_data->data[2] << 16) | (reader_data->data[1] << 8) | reader_data->data[0];

    printf("%x\n", code);
}

void recv_cb(const msg_lvl2_t * msg, void * context) {
    pmd_reader_data_t reader_data;
    uint8_t rc;

    if(msg) {
        rc = pmd_reader_read_data(msg->data, &reader_data);    

        if(!rc) {
            foo(&reader_data);
        }
    }
}

can_net_recv_cb_record_t recv_cb_record;

int main(int argc, char * argv[]) {
    recv_cb_record.check.port_min = 0;
    recv_cb_record.check.port_max = -1;
    recv_cb_record.check.id_min = 0;
    recv_cb_record.check.id_max = -1;
    recv_cb_record.check.smb_min = 0;
    recv_cb_record.check.smb_max = -1;
    recv_cb_record.callback = recv_cb;
    recv_cb_record.cb_ctx = NULL;

	can_net_add_callback(&recv_cb_record);

    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

	printf("sleeping\n");
	sleep(100500);
	printf("the end\n");

	return 0;
}
