#include "contiki.h"
#include "lib/malloc.h"
#include "net/can_net.h"
#include "lib/ftimer.h"
#include "lowlevel.h"

PROCESS(test_process, "");

AUTOSTART_PROCESSES(&test_process);

PROCESS_THREAD(test_process, ev, data) {

    PROCESS_BEGIN();



    PROCESS_END();
}
