#ifndef TIMER_C
#define TIMER_C

/* Timer 2 */
#define OCR2	_SFR_IO8(0x23)
#define TCNT2	_SFR_IO8(0x24)
#define TCCR2	_SFR_IO8(0x25)

/* Timer 1 */
#define ICR1	_SFR_IO16(0x26)
#define ICR1L	_SFR_IO8(0x26)
#define ICR1H	_SFR_IO8(0x27)
#define OCR1B	_SFR_IO16(0x28)
#define OCR1BL	_SFR_IO8(0x28)
#define OCR1BH	_SFR_IO8(0x29)
#define OCR1A	_SFR_IO16(0x2A)
#define OCR1AL	_SFR_IO8(0x2A)
#define OCR1AH	_SFR_IO8(0x2B)
#define TCNT1	_SFR_IO16(0x2C)
#define TCNT1L	_SFR_IO8(0x2C)
#define TCNT1H	_SFR_IO8(0x2D)
#define TCCR1B	_SFR_IO8(0x2E)
#define TCCR1A	_SFR_IO8(0x2F)

#define SFIOR	_SFR_IO8(0x30)

#define OSCCAL	_SFR_IO8(0x31)

/* Timer 0 */
#define TCNT0	_SFR_IO8(0x32)
#define TCCR0	_SFR_IO8(0x33)

#define MCUCSR	_SFR_IO8(0x34)
#define MCUSR   _SFR_IO8(0x34)  /* Defined as an alias for MCUCSR. */

#define MCUCR	_SFR_IO8(0x35)

#define TWCR	_SFR_IO8(0x36)

#define SPMCR	_SFR_IO8(0x37)

#define TIFR	_SFR_IO8(0x38)
#define TIMSK	_SFR_IO8(0x39)

#define GIFR	_SFR_IO8(0x3A)
#define GIMSK	_SFR_IO8(0x3B)
#define GICR	_SFR_IO8(0x3B)   /* Changed in 2486H-AVR-09/02 */

#endif

