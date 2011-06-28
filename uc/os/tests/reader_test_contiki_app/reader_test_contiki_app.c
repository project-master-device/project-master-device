#include "contiki.h"
#include "lib/malloc.h"
#include "net/can_net.h"
#include "lib/ftimer.h"
#include "lowlevel.h"

PROCESS(test_process, "");

AUTOSTART_PROCESSES(&test_process);

void timer_func(void * p) {
    led2_toggle();
    ftimer_register_func(timer_func, NULL, 100);
}

PROCESS_THREAD(test_process, ev, data) {

    PROCESS_BEGIN();

    ftimer_register_func(timer_func, NULL, 100);

    PROCESS_END();
}
