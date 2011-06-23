#define __AVR_ATmega168__
#include <avr/common.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
//#include "WProgram.h"

extern "C" 
void __cxa_pure_virtual(void) {

    for(;;) {
        // error handling code
    }
}

void delay(int i) {
   for(int a = 0; a < i; ++a) 2.84*a;
}

void loop(int i) {
    PORTD &= ~(1 << PD2);
    _delay_ms(1000);

    PORTD |= (1 << PD2);
    _delay_ms(1000);
}

int main() {
    
    DDRD |= (1 << PD2);
    PORTD |= (1 << PD2);

    for(int i = 0; ; ++i) {
        loop(i);
    }

    return 0;
}

