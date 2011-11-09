/*! \file a2d.c \brief Analog-to-Digital converter function library. */
//*****************************************************************************
//
// File Name	: 'a2d.c'
// Title		: Analog-to-digital converter functions
// Author		: Pascal Stang - Copyright (C) 2002
// Created		: 2002-04-08
// Revised		: 2002-09-30
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))

// global variables

//! Software flag used to indicate when
/// the a2d conversion is complete.
volatile unsigned char adc_complete_flag;

// functions

// initialize a2d converter
void adc_init(void)
{
	ADCSR |= (1 << ADEN);				// enable ADC (turn on ADC power)
	ADCSR &= ~(1 << ADFR);				// default to single sample convert mode
	adc_set_prescaler(ADC_PRESCALE);	// set default prescaler
	adc_set_reference(ADC_REFERENCE);	// set default reference
	cbi(ADMUX, ADLAR);				// set to right-adjusted result

	ADCSR |=  (1 << ADIE);				// enable ADC interrupts

	adc_complete_flag = 1;		// clear conversion complete flag
	sei();							// turn on interrupts (if not already on)
}

// turn off a2d converter
void adc_off(void)
{
	ADCSR &= ~(1 << ADIE);				// disable ADC interrupts
	ADCSR &= ~(1 << ADEN);				// disable ADC (turn off ADC power)
}

// configure A2D converter clock division (prescaling)
void adc_set_prescaler(unsigned char prescale)
{
	ADCSR = ((ADCSR & ~ADC_PRESCALE_MASK) | prescale);
}

// configure A2D converter voltage reference
void adc_set_reference(unsigned char ref)
{
	ADMUX = (ADMUX & ~ADC_REFERENCE_MASK) | (ref<<6);
}

// sets the a2d input channel
void adc_set_channel(unsigned char ch)
{
	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (ch & ADC_MUX_MASK);	// set channel
}

// start a conversion on the current a2d input channel
void adc_start_convert(void)
{
	ADCSR |= (1 << ADIF);	// clear hardware "conversion complete" flag
	ADCSR |= (1 << ADSC);	// start conversion
}

// return TRUE if conversion is complete
unsigned char adc_is_complete(void)
{
	return bit_is_set(ADCSR, ADSC);
}

// Perform a 10-bit conversion
// starts conversion, waits until conversion is done, and returns result
unsigned short adc_convert_10bit(unsigned char ch)
{
	adc_complete_flag = 0;				// clear conversion complete flag
	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (ch & ADC_MUX_MASK);	// set channel
	ADCSR |= (1 << ADIF);						// clear hardware "conversion complete" flag
	ADCSR |= (1 << ADSC);						// start conversion
	//while(!adc_complete_flag);				// wait until conversion complete
	//while( bit_is_clear(ADCSR, ADIF) );		// wait until conversion complete
	while( bit_is_set(ADCSR, ADSC) );		// wait until conversion complete

	// CAUTION: MUST READ ADCL BEFORE ADCH!!!
	return ( ADCL | (ADCH<<8));	// read ADC (full 10 bits);
}

// Perform a 8-bit conversion.
// starts conversion, waits until conversion is done, and returns result
unsigned char adc_convert_8bit(unsigned char ch)
{
	// do 10-bit conversion and return highest 8 bits
	return adc_convert_10bit(ch)>>2;			// return ADC MSB byte
}

//! Interrupt handler for ADC complete interrupt.
ISR(SIG_ADC)
{
	// set the a2d conversion flag to indicate "complete"
	adc_complete_flag = 1;
}

