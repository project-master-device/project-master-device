#include "contiki.h"
#include "dev/can.h"
#include "contiki-conf.h"

#include <util/delay.h>
#include <avr/io.h>

void led_toggle();

void init_lowlevel() {
//    DDRC &= (1 << PC1);
//    DDRC |= (1 << PC0);
//    PORTC &= ~(1 << PC0);
    LED1_DDR |= (1 << LED1);
    LED2_DDR |= (1 << LED2);
    LED3_DDR |= (1 << LED3);
    BUTTON_DDR &= ~(1 << BUTTON);
}

PROCINIT(&can_process, &etimer_process);
//PROCINIT(&etimer_process);

void main(void) {
    //?
    clock_init();

    /* Initialize hardware */
    init_lowlevel();

    int i;
    for(i = 0; i < 5; ++i) {
		led1_blink(1, 25);
        led2_blink(1, 25);
        led3_blink(1, 25);
    }

    /* Start process handler */
    process_init();

    /* Register initial processes */
    procinit_init();

    /* Start autostart processes (defined in Contiki application) */
    autostart_start(autostart_processes);

    while (1) {
        process_run();
    }
}

