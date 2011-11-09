#include <avr/common.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {

    DDRB |= (1 << PB6);
    PORTB |= (1 << PB6);

	for(;;) {
        _delay_ms(1000);
        PORTB ^= (1 << PB6);
    }

	return 0;
}

