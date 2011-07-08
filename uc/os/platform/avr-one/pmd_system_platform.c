#include "contiki-conf.h"

#include "lowlevel.h"
#include "lib/ftimer.h"
#include "lib/interrupt.h"

static int enable = 0;

static void identification(void * ctx) {
    interrupt_disable();

    if(enable == 1) {
        led3_toggle();
        ftimer_register_func(identification, NULL, 50);
    }

    interrupt_enable();
}

void pmd_system_platform_identification_on() {
    if(enable == 0) {
        ftimer_register_func(identification, NULL, 50);
        enable = 1;
    }
}

void pmd_system_platform_identification_off() {
    if(enable == 1) {
        led3_off();
        enable = 0;
    }
}
