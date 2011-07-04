#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/system.h"

void send_config_request() {
    msg_lvl2_t msg;
    msg.meta.id = 5;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = 1;
    msg.data.len = 0;
    msg.data.itself = 0;

    pmd_net_system_config_data_t cd;
    cd.operation = PMD_NET_SYSTEM_CONFIG_REQUEST;

    pmd_net_system_config_write_data(&(msg.data), &cd);

    if(msg.data.itself == 0)
        printf("pizdec\n");
    can_net_start_sending_msg(&msg, NULL, NULL);
}

int main(int argc, char * argv[]) {
    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

    send_config_request();

	printf("sleeping\n");
	sleep(100500);
	printf("the end\n");

	return 0;
}
