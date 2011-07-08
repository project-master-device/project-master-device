#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/led/led.h"

void send_handler(const int rc, msg_lvl2_t * msg, void * ctx) {
    printf("send done\n");;
}

int main(int argc, char * argv[]) {
    uint8_t smb = 0;
    uint32_t id;
    pmd_net_led_data_t pmd_led;

    if(argc < 3) {
        printf("Usage: led_command <id> <command>\n");
        printf("id: number\n");
        printf("command: on/off/toggle\n\n");
        return 1;
    }
    sscanf(argv[1], "%d", &id);
    printf("id = %d\n", id);

    if(strcmp(argv[2], "on") == 0) {
        pmd_led.operation = PMD_NET_LED_ON;
    } else if(strcmp(argv[2], "off") == 0) {
        pmd_led.operation = PMD_NET_LED_OFF;
    } else if(strcmp(argv[2], "toggle") == 0) {
        pmd_led.operation = PMD_NET_LED_TOGGLE;
    }
    printf("operation code = %d\n\n", pmd_led.operation);

    uint32_t msg_confirm_tics = 1000;
	int rci = can_net_init(1000, msg_confirm_tics);
	if (rci) {
		printf("initialization return code: %d\n", rci);
		return 0;
	} else
		printf("successful initialization\n");

    msg_lvl2_t msg;
    msg.meta.id = id;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = smb;
    msg.data.itself = NULL;
    msg.data.len = 0;

    pmd_net_led_write_data(&(msg.data), &pmd_led);

    can_net_start_sending_msg(&msg, send_handler, NULL);

	printf("sleeping\n");
	sleep(100500);
	printf("\n the end\n");

	return 0;
}
