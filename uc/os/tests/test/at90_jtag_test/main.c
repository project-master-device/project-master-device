#include <avr/io.h>

int main(void) {
    DDRC  |= (1 << PC0);

	for(;;) {
        PORTC ^= (1 << PC0);
    }

	return 0;
}
