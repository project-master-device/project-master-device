#include <util/delay.h>
#include <avr/io.h>

#include "contiki.h"
#include "lib/malloc.h"
#include "net/can_net.h"
#include "lib/ftimer.h"
#include "lowlevel.h"

PROCESS(init_process, "");
PROCESS(can_net_send_process, "");

AUTOSTART_PROCESSES(&init_process, &can_net_send_process);

int app = -1;
int send_busy = 0;

void send_handler(const int rc, msg_lvl2_t * msg) {
    if(rc == CAN_NET_RC_NORM) {
        led1_blink(1, 500);
    }
    send_busy = 0;
}

PROCESS_THREAD(can_net_send_process, ev, data) {
    static msg_lvl2_t msg;
    static uint8_t msg_buf[64];

    PROCESS_BEGIN();

    msg.meta.hw_addr = 7;
    msg.meta.id = 18;
    msg.meta.port = 1;
    msg.meta.is_system = 0;

    msg.data.itself = msg_buf;
    msg.data.len = 8;
    int i;
    for(i = 0; i < msg.data.len; ++i) {
        msg.data.itself[i] = 0xff;
    }

    for(;;) {
        if((button_state() == BUTTON_ON) && (send_busy == 0)) {
            can_net_start_sending_msg(&msg, send_handler);
            send_busy = 1;
        }
        PROCESS_PAUSE();
    }

    PROCESS_END();
}

void timer_func(void * p) {
    led2_toggle();
    ftimer_register_func(timer_func, NULL, 100);
}

PROCESS_THREAD(init_process, ev, data) {
    PROCESS_BEGIN();

    ftimer_register_func(timer_func, NULL, 100);

    PROCESS_END();
}
