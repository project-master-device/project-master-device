#include <util/delay.h>
#include <avr/io.h>

#include "contiki.h"
#include "lib/malloc.h"
#include "net/can_net.h"
#include "lib/ftimer.h"

PROCESS(init_process, "");
PROCESS(can_net_send_process, "");

AUTOSTART_PROCESSES(&init_process, &can_net_send_process);

void blink(volatile uint8_t * port, uint8_t shift, unsigned int n, unsigned int interval_ms) {
    unsigned int i;

    for(i = 0; i < n; ++i) {
        (*port) |=  (1 << shift);
        _delay_ms(interval_ms);
        (*port) &= ~(1 << shift);
        _delay_ms(interval_ms);
    }
}

void led1_on() {
    LED1_PORT |= (1 << LED1);
}
void led1_off() {
    LED1_PORT &= ~(1 << LED1);
}
void led1_toggle() {
    LED1_PORT ^= (1 << LED1);
}

void led2_on() {
    LED2_PORT |= (1 << LED2);
}
void led2_off() {
    LED2_PORT &= ~(1 << LED2);
}
void led2_toggle() {
    LED2_PORT ^= (1 << LED2);
}

void led3_on() {
    LED3_PORT |= (1 << LED3);
}
void led3_off() {
    LED3_PORT &= ~(1 << LED3);
}
void led3_toggle() {
    LED3_PORT ^= (1 << LED3);
}

void led1_blink(unsigned int n, unsigned int interval_ms) {
    blink(&LED1_PORT, LED1, n, interval_ms);
}
void led2_blink(unsigned int n, unsigned int interval_ms) {
    blink(&LED2_PORT, LED2, n, interval_ms);
}
void led3_blink(unsigned int n, unsigned int interval_ms) {
    blink(&LED3_PORT, LED3, n, interval_ms);
}

uint8_t button_state() {
    return (BUTTON_PIN & (1 << BUTTON)) >> BUTTON;
}

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
    msg.meta.is_system = 0;

    msg.data.itself = msg_buf;
    msg.data.len = 8;
    int i;
    for(i = 0; i < msg.data.len; ++i) {
        msg.data.itself[i] = 0xff;
    }

    for(;;) {
        if((button_state() == BUTTON_ON) && (send_busy == 0)) {
            can_net_start_sending_msg(&msg, 1, send_handler);
            send_busy = 1;
        }
        PROCESS_PAUSE();
    }

    PROCESS_END();
}

void timer_func(void * p) {
    led2_toggle();
    ftimer_register_func(timer_func, 0, 100);
}

PROCESS_THREAD(init_process, ev, data) {
    PROCESS_BEGIN();

    ftimer_register_func(timer_func, 0, 100);

    PROCESS_END();
}
