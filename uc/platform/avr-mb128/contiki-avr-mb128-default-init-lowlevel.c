/*
 * Copyright (c) 2010, MikloBit
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Milosz Klosowicz <support@miklobit.com>
 *
 * @(#)$$
 */
#include "contiki-avr-mb128.h"


void __init_extSRAM(void) __attribute__((naked,section(".init3"))) ;
void __init_extSRAM(void) {


#if defined(__AVR_ATmega128__)
	XDIV  = 0x00; //xtal divider disabled
#  ifdef	EXTERNAL_MEM
	XMCRB = 0x00 ; // 60kb RAM available
	XMCRA = 0x00; //ext ram one sector 1100-FFFF
	MCUCR = ( 1 << SRE ); // ext ram enable
#  endif
#elif defined(__AVR_AT90CAN128__)
	CLKPR = 0x00; // clock divider disabled
# ifdef	EXTERNAL_MEM
	XMCRB = 0x00 ; // 60kb RAM available
#   if MCU_MHZ > 11
	// external devices ( $1100-$7FFF ) -> one wait state
	// external memory ( $8000-$FFFF )  -> zero wait state
	XMCRA = ( 1 << SRE ) | ( 1 << SRL2 ) | ( 1 << SRW00 ) ;
#   else
	XMCRA = ( 1 << SRE ) ; // ext ram enable, one segment, no wait states
#   endif
# endif
#endif
}



//void extRamOn( void ) {
//	XMCRB = 0x00 ; // 60kb RAM available
//#if defined(__AVR_ATmega128__)
//	XDIV  = 0x00; //xtal divider disabled
//	XMCRA = 0x00; //ext ram one sector 1100-FFFF
//	MCUCR = ( 1 << SRE ); // ext ram enable
//#elif defined(__AVR_AT90CAN128__)
//	CLKPR = 0x00; // clock divider disabled
//	XMCRA = ( 1 << SRE ) ; // ext ram enable, one segment
//#endif
//}
//
//void extRamOff( void ) {
//	#if defined(__AVR_ATmega128__)
//		MCUCR &= ~( 1 << SRE ); // ext ram disable
//	#elif defined(__AVR_AT90CAN128__)
//		XMCRA &= ~( 1 << SRE ) ; // ext ram disable
//	#endif
//}


void init_lowlevel(void)
{
  /* Configure external RAM ( 32kb ) */
  // this will be done by __init_extSRAM() in very early init  section(".init3")
  // extRamOn();

#if defined(__AVR_ATmega128__)
  /* Configure default slip port with 115200 baud */
  rs232_init(SLIP_PORT, USART_BAUD_115200, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
#elif defined(__AVR_AT90CAN128__)
#  if MCU_MHZ == 16
  /* Configure default slip port with 1000000 baud */
  /* The non standard baud rate require to use customized ftdi driver settings  on PC side */
#    ifdef  USART_BAUD_1000000
        rs232_init(SLIP_PORT, USART_BAUD_1000000, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
#    else
#      error "you have to define value for 1000000 baud bit rate in cpu/avr/dev/rs232.h"
#    endif
#  else
       rs232_init(SLIP_PORT, USART_BAUD_115200, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
#  endif
#endif

  /* Second rs232 port for debugging */
  rs232_init(DEBUG_PORT, USART_BAUD_115200, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);

  /* Redirect stdout to debug port */
  rs232_redirect_stdout (DEBUG_PORT);

  leds_init();

  adc_init();   // ADC initialization
  adc_set_prescaler( ADC_PRESCALE_DIV2 ); // max possible conversion speed
  adc_set_reference( ADC_REFERENCE_AREF );
}
