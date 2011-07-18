#include <util/delay.h>
#include <avr/io.h>
#include "contiki.h"


PROCESS(blink_process, "Contiki OS port blink test");
PROCESS(led_toggle_process, "Contiki OS port blink test");
PROCESS(button_check_process, "Contiki OS port blink test");

//AUTOSTART_PROCESSES(&led_toggle_process, &button_check_process);
//AUTOSTART_PROCESSES(&blink_process);
AUTOSTART_PROCESSES(NULL);

static process_event_t g_ev;

PROCESS_THREAD(blink_process, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        PORTC ^= (1 << PC0);
        _delay_ms(1000);
    }

    PROCESS_END();
}

PROCESS_THREAD(led_toggle_process, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        PROCESS_WAIT_EVENT();
        PORTC ^= (1 << PC0);
    }

    PROCESS_END();
}

PROCESS_THREAD(button_check_process, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        if(PINC & (1 << PC1)) {
            process_post_synch(&led_toggle_process, g_ev, 0);
        }
        _delay_ms(150);
    }

    PROCESS_END();
}

