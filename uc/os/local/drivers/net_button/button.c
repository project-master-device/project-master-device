#include "contiki.h"
#include "dev/button.h"
#include "lib/ftimer.h"
#include "lib/interrupt.h"

#include <stdint.h>


static process_event_t but_click;
PROCESS(process_handle_button, "Handler button process");


void button_handle_irq(void * conf_sect) {
    process_post(&process_handle_button, but_click, conf_sect);
}

int button_init(config_section_t * conf_sect) {
    volatile uint8_t * ddr = (volatile uint8_t *)config_section_get_uint(conf_sect, "ddr", NULL);
    if(ddr == NULL) return 1;

    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);
    *ddr &= ~(1 << offset);

    process_start(&process_handle_button, NULL);

    //enable button interrupt
    interrupt_register(config_section_get_uint(conf_sect, "offset", 9), INTERRUPT_FALL, button_handle_irq, (void *)conf_sect);

    return 0;
}


PROCESS_THREAD(process_handle_button, ev, data) {
    PROCESS_BEGIN();
    but_click = process_alloc_event();

    for(;;) {
        PROCESS_WAIT_EVENT();
        if (ev == but_click) {
            //TODO send event to master
//            ftimer_register_func(blink, NULL, 2);
            led2_blink(1, 1000);
            interrupt_register(config_section_get_uint((config_section_t *)data, "offset", 9), INTERRUPT_FALL, button_handle_irq, data);
        }
    }

    PROCESS_END();
}
