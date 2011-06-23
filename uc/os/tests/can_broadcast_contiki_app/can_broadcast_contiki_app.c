#include <util/delay.h>
#include <avr/io.h>

#include "lib/malloc.h"
#include "contiki.h"
#include "dev/can.h"

PROCESS(button_check_process, "Contiki OS button check");
PROCESS(can_receiver_process, "Contiki OS can network reciever");
PROCESS(can_transmitter_process, "Contiki OS can network transmit");
PROCESS(test_process, "Contiki OS can network transmit");
PROCESS(test_timer, "Contiki OS can network transmit");
PROCESS(led_blink_process, "");
PROCESS(mask_test_1_proc, "");
PROCESS(mask_test_2_proc, "");

//AUTOSTART_PROCESSES(&led_toggle_process, &button_check_process);
//AUTOSTART_PROCESSES(&can_transmitter_process, &button_check_process);
//AUTOSTART_PROCESSES(&can_transmitter_process, &can_receiver_process, &button_check_process);
//AUTOSTART_PROCESSES(&can_receiver_process, &test_process);
//AUTOSTART_PROCESSES(&can_receiver_process, &test_process, &heap_leak_tester);
//AUTOSTART_PROCESSES(&test_process);
//AUTOSTART_PROCESSES(&test_process, &test_timer);
AUTOSTART_PROCESSES(&mask_test_1_proc, &mask_test_2_proc);
//AUTOSTART_PROCESSES(NULL);


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
    return BUTTON_PIN & (1 << BUTTON);
}


PROCESS_THREAD(button_check_process, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        if(button_state() == 0) {
            process_post_synch(&can_transmitter_process, 0, 0);
        }
        _delay_ms(50);
    }

    PROCESS_END();
}

PROCESS_THREAD(can_receiver_process, ev, data) {
    static uint8_t rx_buffer[8];
    static can_st_cmd_t rx_msg;
    static can_st_cmd_t * cmd_tmp;
    static int i;
    static uint16_t sp;

    PROCESS_BEGIN();

    rx_msg.pt_data = &rx_buffer[0];
    rx_msg.status = 0;
    for(i = 0; i < 8; ++i) {
        rx_buffer[i] = 0;
    }
    rx_msg.p = PROCESS_CURRENT();
    rx_msg.cmd = CAN_CMD_RX;

    for(;;) {
        cmd_tmp = can_post_cmd(&rx_msg);
        PROCESS_WAIT_EVENT();

        if(ev == can_event_message) {
            can_st_cmd_destruct((can_st_cmd_t *)data);
            free(data);
        }

        led3_blink(10, 25);
    }

    PROCESS_END();
}

PROCESS_THREAD(can_transmitter_process, ev, data) {
    static can_st_cmd_t tx_message;
    static uint8_t can_buffer[8];

    PROCESS_BEGIN();

    tx_message.pt_data = can_buffer;
    for(;;) {
        PROCESS_WAIT_EVENT();

        tx_message.ctrl.rtr = 0;
        tx_message.ctrl.ide = 0;
        tx_message.id.std = 34964;
        tx_message.dlc = 1;
        tx_message.pt_data[0] = 2;
        tx_message.p = 0;
        tx_message.cmd = CAN_CMD_TX;

        can_post_cmd(&tx_message);
        led1_blink(1, 100);
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

PROCESS_THREAD(test_timer, ev, data) {
    PROCESS_BEGIN();

    for(;;) {
        _delay_ms(100);
        process_poll(&test_process);
        PROCESS_PAUSE();
    }

    PROCESS_END();
}

PROCESS_THREAD(led_blink_process, ev, data) {
	PROCESS_BEGIN();

	for(;;) {
		led3_toggle();
		_delay_ms(100);
		PROCESS_PAUSE();
	}

	PROCESS_END();
}

PROCESS_THREAD(mask_test_1_proc, ev, data) {
    static uint8_t buf[8];
    static can_st_cmd_t cmd;
    static can_st_cmd_t * cmd_tmp;

    PROCESS_BEGIN();

    cmd.pt_data = buf;
    cmd.p = PROCESS_CURRENT();
    cmd.msk = 0xffffffff;
    cmd.id.std = 1;
    cmd.cmd = CAN_CMD_RX_MASKED;

    for(;;) {
        cmd_tmp = can_post_cmd(&cmd);
        PROCESS_WAIT_EVENT();

        if(ev == can_event_message) {
            can_st_cmd_destruct((can_st_cmd_t *)data);
            free(data);
        }

        led3_blink(10, 25);
    }

    PROCESS_END();
}

PROCESS_THREAD(mask_test_2_proc, ev, data) {
    static uint8_t buf[8];
    static can_st_cmd_t cmd;
    static can_st_cmd_t * cmd_tmp;

    PROCESS_BEGIN();

    cmd.pt_data = buf;
    cmd.p = PROCESS_CURRENT();
    cmd.msk = 0xffffffff;
    cmd.id.std = 0;
    cmd.cmd = CAN_CMD_RX_MASKED;

    for(;;) {
        cmd_tmp = can_post_cmd(&cmd);
        PROCESS_WAIT_EVENT();

        if(ev == can_event_message) {
            can_st_cmd_destruct((can_st_cmd_t *)data);
            free(data);
        }

        led1_blink(10, 25);
    }

    PROCESS_END();
}
