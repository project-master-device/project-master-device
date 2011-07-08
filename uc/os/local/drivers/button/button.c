#include "contiki.h"
#include "dev/button.h"
#include "lib/ftimer.h"
#include "lib/interrupt.h"
#include "net/can_net.h"
#include "pmd_system.h"
#include "pmd_net/button/button.h"

#include <stdint.h>

static uint8_t state = PMD_NET_BUTTON_UP;
static process_event_t but_click;
PROCESS(process_handle_button, "Handler button process");


void button_handle_irq(void * conf_sect) {
    process_post(&process_handle_button, but_click, conf_sect);
}

int button_init(config_section_t * conf_sect) {
    volatile uint8_t * ddr = (volatile uint8_t *)config_section_get_uint(conf_sect, "ddr", (config_uint_t)NULL);
    if(ddr == NULL) return 1;

    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);
    *ddr &= ~(1 << offset);

    process_start(&process_handle_button, NULL);

    //enable button interrupt
    interrupt_register(config_section_get_uint(conf_sect, "offset", 9), INTERRUPT_ANY, button_handle_irq, (void *)conf_sect);

    return 0;
}

static void change_state() {
    switch(state) {
    case PMD_NET_BUTTON_UP:
        state = PMD_NET_BUTTON_DOWN;
        break;
    case PMD_NET_BUTTON_DOWN:
        state = PMD_NET_BUTTON_UP;
        break;
    }
}

PROCESS_THREAD(process_handle_button, ev, data) {
    int rc;
    pmd_net_button_data_t button_data;
    msg_lvl2_t msg;

    PROCESS_BEGIN();
    but_click = process_alloc_event();

    for(;;) {
        PROCESS_WAIT_EVENT();
        if (ev == but_click) {
            change_state();

            button_data.operation = state;

            msg.meta.hw_addr = 7; //FIXME right hw_addr
            msg.meta.port = 1; //FIXME right port
            msg.meta.id = ((config_section_t *)data)->id;
            msg.meta.is_system = 0;
            msg.data.len = 0;
            msg.data.itself = NULL;

            rc = pmd_net_button_write_data(&(msg.data), &button_data);

            if(rc == 0) {
                pmd_system_send_message(&msg, NULL, NULL);
            }

            if(msg.data.itself != NULL) {
                free(msg.data.itself);
                msg.data.len = 0;
                msg.data.itself = NULL;
            }

            interrupt_register(config_section_get_uint((config_section_t *)data, "offset", 9), INTERRUPT_ANY, button_handle_irq, data);
        }
    }

    PROCESS_END();
}
