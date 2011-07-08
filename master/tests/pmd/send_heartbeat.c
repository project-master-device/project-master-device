#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/system_ids.h"

void send_handler(const int rc, msg_lvl2_t * msg, void * ctx) {
    printf("send done\n");;
}

int main(int argc, char * argv[]) {
    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

    msg_lvl2_t msg;
    msg.meta.id = PMD_NET_SYSTEM_HEARTBEAT;
    //msg.meta.id = 432;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = 1;
    msg.data.itself = NULL;
    msg.data.len = 0;

    unsigned long long i = 0;
    unsigned long long max = 1000;
    for(;i < max; ++i) {
        can_net_start_sending_msg(&msg, send_handler, NULL);

        //usleep(50);
    }


    sleep(10);

	return 0;
}
