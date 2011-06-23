#include "contiki.h"
#include "contiki-conf.h"
#include "dev/can_drv.h"
#include "lib/ftimer.h"
#include "pmd_system.h"


PROCINIT(NULL);
//PROCINIT(&can_drv_process, &etimer_process, &ftimer_process);

int main(void) {
    clock_init();

    process_init();
    procinit_init();
    autostart_start(autostart_processes);

    cli();
    pmd_system_init();
    sei();

    while (1) {
        process_run();
    }

    return 0;
}
