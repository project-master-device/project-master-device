#include <util/delay.h>
#include <avr/io.h>

#include "contiki.h"
#include "dev/can.h"


PROCESS(led_toggle_process, "Contiki OS led toggle");
PROCESS(proc1, "Contiki OS test process");
PROCESS(proc2, "Contiki OS test process");

//AUTOSTART_PROCESSES(&led_toggle_process);
AUTOSTART_PROCESSES(&proc1);
//AUTOSTART_PROCESSES(NULL);

void led_toggle(unsigned int n, unsigned int interval_ms) {
    unsigned int i;

    for(i = 0; i < n; ++i) {
        PORTC |= (1 << PC0);
        _delay_ms(interval_ms);
        PORTC &= ~(1 << PC0);
        _delay_ms(interval_ms);
    }
}


PROCESS_THREAD(led_toggle_process, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        led_toggle(1, 1000);
    }

    PROCESS_END();
}

PROCESS_THREAD(proc2, ev, data) {
    static int i;

    PROCESS_BEGIN();

    do {
        led_toggle(1, 1000);

        for(i = 0; i < 0; ++i)
            process_post(&proc1, 1, 0);

//        PROCESS_WAIT_EVENT();
    } while(0);

    PROCESS_END();
}

PROCESS_THREAD(proc1, ev, data) {
    static int i = 0;
    static int pins[] = {PB4, PB5, PB6};

    PROCESS_BEGIN();

    for(;;) {
        if(!(PINB & (1 << PB7))) {
            PORTB |=  (1 << pins[i]);
            _delay_ms(300);
            PORTB &= ~(1 << pins[i]);
            _delay_ms(300);

            i += 1;
            if(i == 3) i = 0;
        }
    }

    PROCESS_END();
}
