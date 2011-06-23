#include <util/delay.h>
#include <avr/io.h>
#include <string.h>

#include "lib/malloc.h"
#include "lib/ftimer.h"
#include "contiki.h"
#include "dev/can_drv.h"

PROCESS(can_send_process, "");
PROCESS(init_process, "");
PROCESS(test_process, "");

AUTOSTART_PROCESSES(&init_process, &test_process, &can_send_process);
//AUTOSTART_PROCESSES(&init_process, &can_send_process);
//AUTOSTART_PROCESSES(&can_init_process, &can_send_process);
//AUTOSTART_PROCESSES(NULL);

int send_busy = 0;

void timer(void * context);

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

static can_frame_t can_frame;

/*
 * returns 1 if equal, 0 if not equal.
 */
int frame_cmp(can_frame_t * cf1, can_frame_t * cf2) {
    int res = 1;
    int i;

    if(res) res = (cf1->inf.err == cf2->inf.err);
    if(res) res = (cf1->inf.ide == cf2->inf.ide);
    if(res) res = (cf1->inf.rtr == cf2->inf.rtr);
    if(res) res = (cf1->inf.id == cf2->inf.id);
    if(res) res = (cf1->data.len == cf2->data.len);
    for(i = 0; res && (i < cf1->data.len); ++i) {
        res = (cf1->data.itself[i] == cf2->data.itself[i]);
    }

    return res;
}

PROCESS_THREAD(can_send_process, ev, data) {
    static uint8_t a = 1;

    PROCESS_BEGIN();

    can_frame.inf.id = 1;
    can_frame.inf.err = 0;
    can_frame.inf.ide = 0;
    can_frame.inf.rtr = 0;

    can_frame.data.len = 8;
    can_frame.data.itself[0] = 0xff;
    can_frame.data.itself[1] = 0x03;
    can_frame.data.itself[2] = 0x00;
    can_frame.data.itself[3] = 0x00;
    can_frame.data.itself[4] = 0x00;
    can_frame.data.itself[5] = 0x00;
    can_frame.data.itself[6] = 0x00;
    can_frame.data.itself[7] = 0x03;

    for(;;) {
        if((button_state() == BUTTON_ON) && (send_busy != 1)) {
            can_send(&can_frame, 0);
            send_busy = 1;
        }
        PROCESS_PAUSE();
    }

    PROCESS_END();
}

void recv_callback(can_frame_t * frame) {
    if(frame_cmp(frame, &can_frame)) {
        led3_blink(1, 500);
        free(frame);
    }
}

void send_callback(const int errcode, can_frame_t * frame, void * context) {
    if(errcode == CAN_DRV_RC_TIMEOUT) {
        led2_blink(1, 1000);
    } else {
        led1_blink(1, 500);
    }
    send_busy = 0;
}

void timer(void * context) {
    led2_toggle();
    ftimer_register_func(timer, 0, 100);
}

PROCESS_THREAD(init_process, ev, data) {
    PROCESS_BEGIN();

    cli();
    EIMSK = 0;
    sei();

    if(can_init(CAN_1000, send_callback, recv_callback, 10000000, 0, 0) == -1) {
        for(;;) {
            led1_blink(1, 25);
            led3_blink(1, 25);
        }
    }

    PROCESS_END();
}

PROCESS_THREAD(test_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();
    etimer_set(&et, CLOCK_SECOND / 10);

    for(;;) {
        PROCESS_WAIT_EVENT();

        led2_toggle();
        etimer_reset(&et);
    }

    PROCESS_END();
}
