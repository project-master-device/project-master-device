#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdio.h>
#include <string.h>
#include "../../../../common/net/can_net.h"
#include "../../../../common/pmd_net/pmd_reader.h"

void recv_cb(msg_lvl2_t * msg) {
    pmd_reader_data_t reader_data;
    int rc;

    if(msg) {
        printf("received message!\n");

        rc = pmd_reader_read_data(msg->data, &reader_data);    

        if(!rc) {
            printf("%x\n", (reader_data.data[2] << 16) | (reader_data.data[1] << 8) | reader_data.data[0]);
        } else {
            printf("Smth bad happend :(\n");
        }
    }
}

can_net_recv_callback_record_t recv_cb_record;

int main(int argc, char * argv[]) {
	can_net_recv_callbacks_arr_t recv_callbacks;
	recv_callbacks.len = 1;
	recv_callbacks.records = &recv_cb_record;

    recv_cb_record.check.port_min = 0;
    recv_cb_record.check.port_max = -1;
    recv_cb_record.check.id_min = 0;
    recv_cb_record.check.id_max = -1;
    recv_cb_record.check.smb_min = 0;
    recv_cb_record.check.smb_max = -1;
    recv_cb_record.callback = recv_cb;

	can_net_add_callbacks(recv_callbacks);

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
