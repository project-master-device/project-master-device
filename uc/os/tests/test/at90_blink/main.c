#include <avr/common.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

int main() {

    DDRC |= (1 << PC0);
    PORTC |= (1 << PC0);

	for(;;) {
        _delay_ms(1000);
        PORTC ^= (1 << PC0);
    }

	return 0;
}

