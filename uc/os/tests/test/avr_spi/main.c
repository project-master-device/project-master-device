#include <util/delay.h>
#include <avr/common.h>
#include <avr/io.h>

#define SPIDDR DDRB

#define SPISCK PB7
#define SPIMOSI PB5
#define SPIMISO PB6
#define SPISS PB3

void spi_init() {
    SPIDDR |= (1 << SPISCK);
    SPIDDR |= (1 << SPIMOSI);
    SPIDDR |= (1 << SPISS);
	SPIDDR &= ~(1 << SPIMISO);
    
    SPCR = (1<<SPE) | (1<<MSTR);
}

void main() {
    spi_init();

    for (;;) _delay_ms(100);
}

