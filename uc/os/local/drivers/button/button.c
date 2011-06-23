#include "lib/config.h"
#include "dev/button.h"
#include "lib/ftimer.h"
#include "lib/malloc.h"
#include "lib/interrupt.h"
#include "sys/process.h"
#include <stdint.h>

process_event_t but_click;
PROCESS(process_handle_button, "Handler button process");
void blink(void *);

/**
 * @struct button_t
 * It contains pointers to functions which must be realized in specific platform.
 * They must be set in initializations of devices.
 */
typedef struct button_type {
    config_section_t * param;
} button_t;


void button_init(button_t * but) {
    volatile uint8_t * ddr = (volatile uint8_t *)config_section_get_uint(but->param, "ddr", 0);
    uint8_t offset = config_section_get_uint(but->param, "offset", 0);

    *ddr &= ~(1 << offset);
}

void button_handle_irq(void * but_) {
    process_post(&process_handle_button, but_click, but_);
}

void button_create (config_section_t * sect) {
    button_t * but = (button_t *)malloc(sizeof(button_t));

    but->param = sect;

    button_init(but);
    process_start(&process_handle_button, NULL);

    //enable button interrupt
    interrupt_register(config_section_get_uint(but->param, "offset", 9), INTERRUPT_ANY, button_handle_irq, (void *)but);
}


PROCESS_THREAD(process_handle_button, ev, data) {
    button_t * but;

    PROCESS_BEGIN();
    but_click = process_alloc_event();

    for(;;) {
        PROCESS_WAIT_EVENT();
        if (ev == but_click) {
            //TODO send event to master
//            ftimer_register_func(blink, NULL, 2);

            but = (button_t *)data;
            interrupt_register(config_section_get_uint(but->param, "offset", 9), INTERRUPT_ANY, button_handle_irq, data);
        }
    }

    PROCESS_END();
}
