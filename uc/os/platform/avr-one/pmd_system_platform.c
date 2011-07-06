#include "contiki-conf.h"

#include "lowlevel.h"
#include "lib/ftimer.h"

static int id = -1;

static void identification(void * ctx) {
    led3_toggle();
    ftimer_register_func(identification, NULL, 1000);
}

void pmd_system_platform_identification_on() {
    if(id == -1) {
        id = ftimer_register_func(identification, NULL, 1000);
    }
}

void pmd_system_platform_identification_off() {
    if(id != -1) {
        ftimer_unregister_func(id);
        id = -1;
    }
}
