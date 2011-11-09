#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "devices.h"

void timer_init() {
    ASSR |= (1 << AS2);
    TCCR2A |= (1 << CS20);
    TIMSK2 |= (1 << TOIE2);
}

ISR(TIMER2_OVF_vect) {
    led2_blink(1, 200);
}

void main(void) {
    devices_init();
    cli();
    timer_init();
    sei();

    for(;;) {
        led1_blink(1, 1000);
    }
}
