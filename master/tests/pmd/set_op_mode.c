#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/system/system_ids.h"
#include "../../../common/pmd_net/system/set_op.h"

void send_handler(const int rc, msg_lvl2_t * msg, void * ctx) {
    printf("send done\n");;
}

int main(int argc, char * argv[]) {
    if(argc < 2) {
        printf("Usage: set_op <op.mode>\n");
        printf("op.mode: init/norm/conf\n\n");
        return 1;
    }

    pmd_net_sys_set_op_data_t opd;

    if(strcmp(argv[1], "init") == 0) {
        opd.operation = PMD_NET_SYS_SET_OP_INIT;
    } else if(strcmp(argv[1], "norm") == 0) {
        opd.operation = PMD_NET_SYS_SET_OP_NORMAL;
    } else if(strcmp(argv[1], "conf") == 0) {
        opd.operation = PMD_NET_SYS_SET_OP_CONFIGURATION;
    }

    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

    msg_lvl2_t msg;
    msg.meta.id = PMD_NET_SYS_SET_OP;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = 1;

    if(pmd_net_sys_set_op_write_data(&(msg.data), &opd) != 0) {
        printf("epic fail :(\n");
    }

    can_net_start_sending_msg(&msg, send_handler, NULL);

	printf("sleeping\n");
	sleep(100500);
	printf("\n the end\n");

	return 0;
}
