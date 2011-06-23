#include <util/delay.h>
#include <avr/common.h>
#include <avr/io.h>

#define SPIDDR DDRB

#define SPISCK PB5
#define SPIMOSI PB3
#define SPIMISO PB4
#define SPISS PB2

void spi_init() {
    SPIDDR |= (1 << SPISCK);
    SPIDDR |= (1 << SPIMOSI);
    SPIDDR |= (1 << SPISS);
	SPIDDR &= ~(1 << SPIMISO);
    
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_readwrite(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}

void main() {
    spi_init();

    for (;;) { 
        spi_readwrite(231);
    }
}

