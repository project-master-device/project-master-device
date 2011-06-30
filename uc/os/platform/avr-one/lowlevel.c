#include <util/delay.h>

#include "lowlevel.h"


void lowlevel_init() {
    LED1_DDR |= (1 << LED1);
    LED2_DDR |= (1 << LED2);
    LED3_DDR |= (1 << LED3);
    BUTTON_DDR &= ~(1 << BUTTON);
}

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
