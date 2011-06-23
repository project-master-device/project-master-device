/******************************************************************************
 * Copyright (C) 2005 Martin THOMAS, Kaiserslautern, Germany
 * <eversmith@heizung-thomas.de>
 * http://www.siwawi.arubi.uni-kl.de/avr_projects
 *****************************************************************************
 *
 * File    : spi.c
 * Version : 0.9
 * 
 * Summary : AVR SPI driver
 *
 *****************************************************************************/

#include <avr/io.h>
#include "spi.h"

#define SPIDONTCARE (0x00)

/* init as SPI-Master */
void spi_init(void)
{
	// SCK, SS!!, MOSI as outputs
    SPIDDR |= (1 << SPISCK);
    SPIDDR |= (1 << SPIMOSI);
    SPIDDR |= (1 << SPISS);

    // MISO as input
	//SPIDDR &= ~(1 << SPIMISO);	// cbi(SPIDDR,SPIMISO);
	// INIT interface, Master, set clock rate fck/128 TODO: check prescaler
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);

    SPIPORT &= ~(1 << SPISCK);
    SPIPORT &= ~(1 << SPIMOSI);
    SPIPORT |=  (1 << SPISS);

    SPCR |= (1 << MSTR);
    SPCR |= (1 << SPE);
}

uint8_t spi_readwrite(uint8_t data)
{
	// set data to send into SPI data register
	SPDR = data;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
	// return data read from SPI
	return SPDR;
}

uint8_t spi_read(void)
{
	return spi_readwrite(SPIDONTCARE);
}
