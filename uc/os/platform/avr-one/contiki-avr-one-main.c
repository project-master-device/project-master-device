#include "contiki.h"
#include "contiki-conf.h"
#include "net/can_drv.h"
#include "lib/ftimer.h"
#include "pmd_system.h"
#include "lowlevel.h"
#include <util/delay.h>

//PROCINIT(NULL);
PROCINIT(&can_drv_process, &etimer_process, &ftimer_process);

int main(void) {
    clock_init();

    process_init();
    procinit_init();
    autostart_start(autostart_processes);

//    cli();
//    pmd_system_init();
//    sei();

    lowlevel_init();

    int i;
    for(i = 0; i < 5; ++i) {
        led1_blink(1, 25);
        led2_blink(1, 25);
        led3_blink(1, 25);
    }

    while (1) {
        process_run();
    }

    return 0;
}
