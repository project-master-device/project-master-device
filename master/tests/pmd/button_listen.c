#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/button/button.h"

void recv_cb(const msg_lvl2_t * msg, void * ctx) {
    pmd_net_button_data_t button_data;
    int rc;

    if(msg) {
        rc = pmd_net_button_read_data(&(msg->data), &button_data);

        if(!rc) {
            if(button_data.operation == PMD_NET_BUTTON_DOWN)
                printf("Button down\n");
            else if (button_data.operation == PMD_NET_BUTTON_UP)
                printf("Button up\n");

        } else {
            printf("Smth bad happend :(\n");
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
