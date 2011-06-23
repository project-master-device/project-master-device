#include <avr/common.h>
#include <avr/io.h>

#if defined (__AVR_ATmega168__)
#define SPIPORT PORTB
#define SPIDDR DDRB
#define SPISCK PB5
#define SPIMOSI PB3
#define SPIMISO PB4
#define SPISS PB2
#elif defined (__AVR_ATmega162__)
#define SPIPORT PORTB
#define SPIDDR  DDRB
#define SPISCK  PB7
#define SPIMOSI PB5
#define SPIMISO PB6
#define SPISS   PB4
#else
#warning "device type not defined"
#endif

void spi_init(void);
uint8_t spi_transfer_one(uint8_t val);
void spi_transfer(uint8_t* send_buf, uint8_t* recv_buf, uint8_t len);

