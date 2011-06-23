#include "spi.h"

void spi_init() {
    SPIDDR |= (1 << SPISCK);
    SPIDDR |= (1 << SPIMOSI);
    SPIDDR |= (1 << SPISS);
    SPIDDR &= ~(1 << SPIMISO);

    SPIPORT &= ~(1 << SPISCK);
    SPIPORT &= ~(1 << SPIMOSI);
    SPIPORT |= (1 << SPISS);


    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t spi_transfer_one(uint8_t val) {
	SPDR = val;
    while(!(SPSR & (1<<SPIF)));
	return SPDR;
}


void spi_transfer(uint8_t* send_buf, uint8_t* recv_buf, uint8_t len) {
    int i;
    for(i = 0; i < len; ++i) { 
        recv_buf[i] = spi_transfer_one(send_buf[i]);
    }
}

