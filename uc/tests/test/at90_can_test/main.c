#include <util/delay.h>
#include <avr/io.h>

#include "can.h"

#define LPCK_DDR  DDRC
#define LPCK_PORT PORTC
#define LPCK PC0
#define BUTTON_DDR DDRC
#define BUTTON_PIN PINC
#define BUTTON PC1

void led_init(void) {
    LPCK_DDR |= (1 << LPCK);
}

void button_init(void) {
    BUTTON_DDR &= ~(1 << BUTTON);
}

void led_toggle(uint8_t n, uint32_t interval_ms) {
    while(n) {
        LPCK_PORT |= (1 << LPCK);
        _delay_ms(interval_ms);
        LPCK_PORT &= ~(1 << LPCK);
        _delay_ms(interval_ms);
        n--;
    }
}

uint8_t get_button_state(void) {
    return BUTTON_PIN & (1 << BUTTON);
}


void main(void) {
    led_init();
    button_init();
    can_init();
    can_config_rate();

    int cur_mob = -1;
    int recv_mob = -1;
    can_frame cf_recv;
    can_frame cf_to_send;
    cf_to_send.ctrl.ide = 0;
    cf_to_send.ctrl.rtr = 0;
    cf_to_send.dlc = 1;
    cf_to_send.id.std = 3;
    cf_to_send.data[0] = 4;
    recv_mob = can_en_receive_mob();

    for(;;) {
#if 0 // CANREC value
        led_toggle(CANREC, 1000);
#endif
#if 0// CANTEC value
        led_toggle(CANTEC, 1000);
#endif
#if 0 // CANSTMOB bits
        if(cur_mob != -1) {
            can_set_mob(cur_mob);
            led_toggle((CANSTMOB & (1 << RXOK)) >> RXOK, 1000);
        }
#endif
#if 0 // CANGSTA bits
        while( (CANGSTA & (1 << OVRG)) >> OVRG) {
            led_toggle(1, 1000);
        }
#endif
#if 0 // rx
        if(recv_mob == -1) {
            recv_mob = can_en_receive_mob();
        } else {
            can_set_mob(recv_mob);
            if(can_mob_get_rx_status()) {
                can_read_frame(&cf_recv);
                led_toggle(cf_recv.data[0], 300);
                recv_mob = can_en_receive_mob();
            }
        }
#endif
#if 0 // tx
        if(get_button_state()) {
            cur_mob = can_send_frame(&cf_to_send);
            if(cur_mob != -1) led_toggle(1, 100);
        }
#endif
        _delay_ms(100);
    }
}
