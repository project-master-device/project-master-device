#define CANSOCKET_LOG
#define CANSOCKET_LOOPBACK
#define CAN_NET_VALIDATE_MSGS
#define CAN_NET_LOWLVL_FUNCS
#define CAN_NET_CONFIRMATION

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../../common/net/can_net.h"
#include "../../../common/pmd_net/system_ids.h"
#include "../../../common/pmd_net/system_config.h"

void send_config_request() {
    msg_lvl2_t msg;
    msg.meta.id = PMD_NET_SYSTEM_CONFIG_MSG;
    msg.meta.hw_addr = 123;
    msg.meta.port = 1;
    msg.meta.is_system = 1;
    msg.data.len = 0;
    msg.data.itself = 0;

    pmd_net_system_config_data_t cd;
    cd.operation = PMD_NET_SYSTEM_CONFIG_REQUEST;
    cd.config = NULL;
    cd.section = NULL;

    pmd_net_system_config_write_data(&(msg.data), &cd);

    if(msg.data.itself == 0)
        printf("pizdec\n");
    can_net_start_sending_msg(&msg, NULL, NULL);
}


void config_print(config_cnf_t * cnf) {
    config_section_t * sect_it = NULL;
    config_option_t * opt_it = NULL;
    char buf[255];

    for(sect_it = (config_section_t *) list_head(cnf->sections); sect_it != NULL; sect_it = (config_section_t *) list_item_next(sect_it)) {
        printf("-- section: %u (at address %p) --\n", sect_it->id, sect_it);
        printf("next address: %p\n", sect_it->next);
        printf("options:\n");
        for(opt_it = (config_option_t *) list_head(sect_it->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
            printf("%p\n", opt_it);
            printf("\t%s=", opt_it->key);

            switch(opt_it->type) {
                case 's':
                    config_section_get_str(sect_it, opt_it->key, "<default val>", buf, 255);
                    printf("%s", buf);
                    break;

                case 'i':
                    printf("%d", config_section_get_int(sect_it, opt_it->key, -100));
                    break;

                case 'I':
                    printf("%u", config_section_get_uint(sect_it, opt_it->key, 100));
                    break;

                default:
                    printf("<NOT CORRECT VALUE>");
                    break;
            }

            printf("\n\tsize = %d\n", opt_it->size);
            printf("\tnext address: %p\n", opt_it->next);
        }
        printf("\n");
    }
}

void recv_cb(const msg_lvl2_t * msg, void * context) {
    pmd_net_system_config_data_t cd;

    if(msg) {
        printf("received message!\n");

        if(msg->meta.id == PMD_NET_SYSTEM_CONFIG_MSG) {
            if(pmd_net_system_config_read_data(&(msg->data), &cd) != 0) {
                printf("fail: failed to read message data\n");
                return;
            }

            if(cd.config != NULL)
                config_print(cd.config);
            else
                printf("fail: config pointer is null");
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

    send_config_request();

	printf("sleeping\n");
	sleep(100500);
	printf("the end\n");

	return 0;
}
