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
 * 3  Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE copyright holders AND CONTRIBUTORS ``AS IS'' AND
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
 * @(#)$
 */

/**
 * \file
 *       USB(serial)/CAN converter .
 *       constant CAN_BAUDRATE has to be defined in contiki-conf.h
 *
 * \author
 *       Milosz Klosowicz <support@miklobit.com>
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "canconverter.h"
#include "dev/can.h"
#include "dev/rs232.h"
#include "dev/leds.h"

PROCESS(can_converter_process, "USB/CAN converter");

#define RX_BUFSIZE 128

#define CANCONVERTER_OPEN  1
#define CANCONVERTER_CLOSE  0

#define CANCONVERTER_CMD_END '\r'
#define CANCONVERTER_CMD_OK '\r'
#define CANCONVERTER_CMD_ERROR '\a'
#define CANCONVERTER_CMD_SPEED_STANDARD 'S'
#define CANCONVERTER_CMD_SPEED_VAR 's'
#define CANCONVERTER_CMD_OPEN 'O'
#define CANCONVERTER_CMD_CLOSE 'C'
#define CANCONVERTER_CMD_TX_11BIT 't'
#define CANCONVERTER_CMD_TX_29BIT 'T'
#define CANCONVERTER_CMD_GET_STATUS 'F'
#define CANCONVERTER_CMD_SET_RX_ID 'M'
#define CANCONVERTER_CMD_SET_RX_MSK 'm'
#define CANCONVERTER_CMD_GET_VERSION 'V'
#define CANCONVERTER_CMD_GET_SERNUM 'N'
#define CANCONVERTER_CMD_SET_TIMESTAMP 'Z'

#define CANCONVERTER_VERSION_NUM  PSTR ("V1010\r")
#define CANCONVERTER_SERIAL_NUM   PSTR ("N0001\r")

// input buffer state
enum {
  STATE_TWOPACKETS = 0,	/* We have 2 packets and drop incoming data. */
  STATE_OK = 1,
  STATE_RUBBISH = 2
};
static u8_t state = STATE_TWOPACKETS;


static uint8_t can_open = CANCONVERTER_CLOSE ;

static u16_t begin, end;
static u8_t rxbuf[RX_BUFSIZE];
static u8_t cmdbuf[RX_BUFSIZE];
static u8_t cmdlen ;
static u16_t pkt_end;

static uint8_t can_timestamp_send = 1;


#ifdef CAN_BAUDRATE_VARIABLE
static uint8_t can_speed = CAN_S3 ;   // default = 100Kbit
#else
static uint8_t can_speed = 0 ;
#endif
//--- status of last operation
static uint8_t can_status = 0 ;
static uint32_t can_accept_id = 0 ;
static uint32_t can_accept_msk = ~0 ;
static uint8_t can_rx_buffer[8];
static can_st_cmd_t can_rx_msg;
static uint8_t can_tx_buffer[8];
static can_st_cmd_t can_tx_msg;
static uint8_t can_message_buf[128];


/*-----------------------------------------------------------------------------------*/
static void rxbuf_init(void)
{
  begin = end = pkt_end = 0;
  state = STATE_OK;
}
/*-----------------------------------------------------------------------------------*/
static uint8_t htoi( uint8_t val ) {
	if( val >= '0' && val <= '9' ) {
		return (val - '0');
	}
	else {
	    return ( toupper( val ) - 'A' + 10 );
	}
}

static void canconverter_config_rx( void )
{
	can_rx_msg.status = 0;
	// can_rx_msg.id.std = (u16_t)can_accept_id;
	// can_rx_msg.id.ext = can_accept_id;
	can_rx_msg.id.ext = 0 ;
	// can_rx_msg.id.std = 0;
	can_rx_msg.ctrl.ide = 1;
	//can_rx_msg.msk = ~can_accept_msk;
	can_rx_msg.msk = 0 ;
	// can_rx_msg.ctrl.rtr = 0;
	can_rx_msg.dlc = 8;
	can_rx_msg.p = PROCESS_CURRENT();    // command will trigger event when complete
	can_rx_msg.cmd = CAN_CMD_RX_MASKED;
	can_cmd(&can_rx_msg);
}
/*-----------------------------------------------------------------------------------*/
static void canconverter_can_open(uint8_t is_open)
{
	can_open = is_open ;
	if( can_open )
	{
	    leds_on( LEDS_LED2 );
	}
	else
	{
	    leds_off( LEDS_LED2 );
	}
}

/*-----------------------------------------------------------------------------------*/
static u8_t canconverter_copy_buffer()
{
	u8_t *bufptr = cmdbuf ;
  /*
   * Interrupt can not change begin but may change pkt_end.
   * If pkt_end != begin it will not change again.
   */
	if(begin != pkt_end) {
		u8_t len;

		if(begin < pkt_end)
		{
			len = pkt_end - begin;
			memcpy(bufptr, &rxbuf[begin], len);
		}
		else
		{
			len = (RX_BUFSIZE - begin) + (pkt_end - 0);
			u8_t i;
			for(i = begin; i < RX_BUFSIZE; i++)
			{
				*bufptr++ = rxbuf[i];
			}
			for(i = 0; i < pkt_end; i++)
			{
				*bufptr++ = rxbuf[i];
			}
		}

		/* Remove data from buffer together with the copied packet. */
		begin = pkt_end;
		if(state == STATE_TWOPACKETS) {
			pkt_end = end;
			state = STATE_OK;		/* Assume no bytes where lost! */

			/* One more packet is buffered, need to be polled again! */
			process_poll(&can_converter_process);
		}
		return len;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int canconverter_input_byte(unsigned char c)
{
	switch(state) {
	case STATE_RUBBISH:
		if(c == CANCONVERTER_CMD_END) {
			state = STATE_OK;
		}
		return 0;

	case STATE_TWOPACKETS:       /* Two packets are already buffered! */
		return 0;

	case STATE_OK:
		if(c == CANCONVERTER_CMD_END) {
			/*
			 * We have a new packet, possibly of zero length.
			 *
			 * There may already be one packet buffered.
			 */
			if(end != pkt_end)
			{	/* Non zero length. */
				if(begin == pkt_end)
				{	/* None buffered. */
					pkt_end = end;
				} else {
					state = STATE_TWOPACKETS;
				}
				process_poll(&can_converter_process);
				return 1;
			}
			else
			{
				process_poll(&can_converter_process); // zero length packet
				return 0;
			}

		}
		break;
	}

	/* add_char: */
	{
		u16_t next;
		next = end + 1;
		if(next == RX_BUFSIZE) {
			next = 0;
		}
		if(next == begin) {		/* rxbuf is full */
			state = STATE_RUBBISH;
			end = pkt_end;		/* remove rubbish */
			return 0;
		}
		rxbuf[end] = c;
		end = next;
	}
	return 0;
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_open( void )
{
	if( can_open != CANCONVERTER_CLOSE ) {
		canconverter_can_open(CANCONVERTER_CLOSE);
		can_rx_msg.cmd = CAN_CMD_ABORT;
		can_cmd(&can_rx_msg);
	}
	if( can_init( can_speed ) == 0 ) {
		rs232_send (CAN_PORT, CANCONVERTER_CMD_ERROR );
		return ;
	}
	canconverter_can_open(CANCONVERTER_OPEN);
	canconverter_config_rx();
	rs232_send (CAN_PORT, CANCONVERTER_CMD_END );

}
/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_close( void )
{
	if( can_open != CANCONVERTER_CLOSE )
	{
		canconverter_can_open(CANCONVERTER_CLOSE);
		can_rx_msg.cmd = CAN_CMD_ABORT;
		can_cmd(&can_rx_msg);
		rs232_send (CAN_PORT, CANCONVERTER_CMD_END );
	}
	else
	{
		rs232_send (CAN_PORT, CANCONVERTER_CMD_ERROR );
	}

}
/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_speed_standard( void ) {
#ifdef CAN_BAUDRATE_VARIABLE
	uint8_t result = CANCONVERTER_CMD_ERROR ;
	uint8_t speed ;
	if( can_open == CANCONVERTER_CLOSE )
	{
	   if( cmdlen == 2) {
		  speed = cmdbuf[1] - '0' ;
		  if( (speed >= CAN_S0) && (speed <= CAN_S8) )
		  {
			  can_speed = speed ;
			  result = CANCONVERTER_CMD_OK ;
		  }
	   }
	}
	rs232_send (CAN_PORT, result );
#else
	rs232_send (CAN_PORT, CANCONVERTER_CMD_OK );
#endif

}
/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_get_version( void ) {
	rs232_print_p (CAN_PORT, CANCONVERTER_VERSION_NUM );
}
/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_get_serial( void ) {
	rs232_print_p (CAN_PORT, CANCONVERTER_SERIAL_NUM );
}
/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_get_status( void ) {
	if( can_open != CANCONVERTER_CLOSE )
	{
		if( can_status != 0 ) {
			sprintf(can_message_buf,"F%02x\r",can_status);
			rs232_print (CAN_PORT, can_message_buf);
		}
		else
			rs232_print_p (CAN_PORT, PSTR ("F00\r"));
	}
	else
	{
		rs232_send (CAN_PORT, CANCONVERTER_CMD_ERROR );
	}
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_tx_11bit( void )
{
	uint8_t result = CANCONVERTER_CMD_ERROR ;
	uint8_t len ;
	uint16_t id ;
	uint8_t cnt ;
	if( (cmdlen > 4) && (can_open != CANCONVERTER_CLOSE) ) // minimal frame len with 0 byte
	{
		len = cmdbuf[4] - '0';
		if( ((2 * len + 5) == cmdlen) && (len <= 8)  ) {
			id = ( htoi(cmdbuf[1]) << 8 ) | ( htoi(cmdbuf[2]) << 4 ) |  htoi(cmdbuf[3])  ;
			// abort previous tx message if not already sent
			if( can_tx_msg.status == MOB_PENDING )
			{
			   can_tx_msg.cmd = CAN_CMD_ABORT;
			   can_cmd(&can_tx_msg);
			}
			can_tx_msg.dlc = len ;
			for( cnt = 0; cnt < len; cnt ++ )
			{
				can_tx_buffer[cnt] = ( htoi(cmdbuf[5 + (cnt << 1)]) << 4 ) | htoi(cmdbuf[6 + (cnt << 1)]) ;
			}
			can_tx_msg.ctrl.ide = 0;            //- CAN 2.0A
			can_tx_msg.id.std = id;
			can_tx_msg.p = NULL ;   			  // command complete  don't send event to us
			can_tx_msg.cmd = CAN_CMD_TX_DATA;
		    can_cmd(&can_tx_msg);
		    result = CANCONVERTER_CMD_OK ;
		}
	}
	rs232_send (CAN_PORT, result );
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_tx_29bit( void )
{
	uint8_t result = CANCONVERTER_CMD_ERROR ;
	uint8_t len ;
	uint32_t id ;
	uint8_t cnt ;
	if( (cmdlen > 9) && (can_open != CANCONVERTER_CLOSE)) // minimal frame len with 0 byte
	{
		len = cmdbuf[9] - '0';
		if( ((2 * len + 10) == cmdlen) && ( len <= 8 )  ) {
			id = 0;
			for( cnt = 1; cnt <= 8; cnt ++ )
			{
				id = (id << 4) | htoi(cmdbuf[cnt]) ;
			}
			// abort previous tx message if not already sent
			if( can_tx_msg.status == MOB_PENDING )
			{
			   can_tx_msg.cmd = CAN_CMD_ABORT;
			   can_cmd(&can_tx_msg);
			}
			can_tx_msg.dlc = len ;
			for( cnt = 0; cnt < len; cnt ++ )
			{
				can_tx_buffer[cnt] = ( htoi(cmdbuf[10 + (cnt << 1)]) << 4 ) | htoi(cmdbuf[11 + (cnt << 1)]) ;
			}
			can_tx_msg.ctrl.ide = 1;            //- CAN 2.0B
			can_tx_msg.id.ext = id;
			can_tx_msg.p = NULL ;   			  // command complete  don't send event to us
			can_tx_msg.cmd = CAN_CMD_TX_DATA;
		    can_cmd(&can_tx_msg);
		    result = CANCONVERTER_CMD_OK ;
		}
	}
	rs232_send (CAN_PORT, result );
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_set_timestamp( void )
{
	uint8_t result = CANCONVERTER_CMD_ERROR ;
	if( can_open == CANCONVERTER_CLOSE )
	{
	   if( cmdlen == 2) {
		   can_timestamp_send = cmdbuf[1] - '0' ;
		   result = CANCONVERTER_CMD_OK ;
	   }
	}
	rs232_send (CAN_PORT, result );
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_cmd_set_value_32bit( uint32_t *value )
{
	uint8_t result = CANCONVERTER_CMD_ERROR ;
	uint32_t id = 0 ;
	if( can_open == CANCONVERTER_CLOSE )
	{
	   if( cmdlen == 9) {
		   // example: M00112233 -> set acceptance id to 0x33221100
		   id = ( htoi(cmdbuf[1] << 4) | htoi(cmdbuf[2]) )   |
		        ((uint32_t)( htoi(cmdbuf[3] << 4) | htoi(cmdbuf[4]) ) << 8 ) |
		        ((uint32_t)( htoi(cmdbuf[5] << 4) | htoi(cmdbuf[5]) ) << 16 ) |
		        ((uint32_t)( htoi(cmdbuf[7] << 4) | htoi(cmdbuf[7]) ) << 24 ) ;
		   *value = id;
		   result = CANCONVERTER_CMD_OK ;
	   }
	}
	rs232_send (CAN_PORT, result );
}

/*-----------------------------------------------------------------------------------*/
static void canconverter_handle_command( void )
{

	if( cmdlen == 0 )
	{
		rs232_send (CAN_PORT, CANCONVERTER_CMD_END );
	}
	else
	{
		switch(cmdbuf[0]) {
		case CANCONVERTER_CMD_GET_STATUS: 		canconverter_cmd_get_status();  break ;
		case CANCONVERTER_CMD_TX_11BIT: 		canconverter_cmd_tx_11bit();  break ;
		case CANCONVERTER_CMD_TX_29BIT: 		canconverter_cmd_tx_29bit();  break ;
		case CANCONVERTER_CMD_OPEN: 			canconverter_cmd_open() ; break ;
		case CANCONVERTER_CMD_CLOSE: 			canconverter_cmd_close(); break ;
		case CANCONVERTER_CMD_SPEED_STANDARD: 	canconverter_cmd_speed_standard(); break ;
		case CANCONVERTER_CMD_GET_VERSION : 	canconverter_cmd_get_version(); break ;
		case CANCONVERTER_CMD_SET_RX_ID : 		canconverter_cmd_set_value_32bit(&can_accept_id); break ;
		case CANCONVERTER_CMD_SET_RX_MSK : 		canconverter_cmd_set_value_32bit(&can_accept_msk); break ;
		case CANCONVERTER_CMD_SET_TIMESTAMP : 	canconverter_cmd_set_timestamp(); break ;
		case CANCONVERTER_CMD_GET_SERNUM: 		canconverter_cmd_get_serial(); break ;
		};
	}
}

static void canconverter_handle_rx_data( void ) {
	  static uint8_t *ptr;
	  static uint8_t cnt;

	  ptr = &can_message_buf[0];

	  if (can_rx_msg.ctrl.ide)    // 29 bit frame
	  {
		  *(ptr++) = CANCONVERTER_CMD_TX_29BIT ;
		  sprintf(ptr,"%04x",(can_rx_msg.id.ext >> 16) );
		  ptr += 4;
		  sprintf(ptr,"%04x",(can_rx_msg.id.ext & 0xffff) );
		  ptr += 4;
	  }
	  else     				// 11 bit frame
	  {
		  *(ptr++) = CANCONVERTER_CMD_TX_11BIT ;
		  sprintf(ptr,"%03x",can_rx_msg.id.std );
		  ptr += 3;
	  }

	  *(ptr++) = '0' + can_rx_msg.dlc ;
	  for( cnt = 0; cnt < can_rx_msg.dlc; cnt ++ ) {
		  sprintf(ptr,"%02x",can_rx_buffer[cnt]);
		  ptr += 2;
	  }
	  *(ptr++) = CANCONVERTER_CMD_END ;
	  *ptr = 0;
	  rs232_print (CAN_PORT, can_message_buf);

	  // rs232_print_p (CAN_PORT, PSTR ("tfff8ffffffffffffffff\r"));

	  can_rx_msg.cmd = CAN_CMD_ABORT;
	  can_cmd(&can_rx_msg);

	  canconverter_config_rx();
}


/*-----------------------------------------------------------------------------------*/
PROCESS_THREAD(can_converter_process, ev, data)
{

  PROCESS_BEGIN();

  rxbuf_init();
  canconverter_can_open(CANCONVERTER_CLOSE);
  rs232_set_input (CAN_PORT, canconverter_input_byte);
  rs232_print_p (CAN_PORT, PSTR ("\r"));

  can_rx_msg.pt_data = &can_rx_buffer[0];
  can_tx_msg.pt_data = &can_tx_buffer[0];

  while(1)
  {
	  PROCESS_WAIT_EVENT();
	  if(ev == PROCESS_EVENT_POLL) {
		  cmdlen = canconverter_copy_buffer();
		  canconverter_handle_command();
	  }
	  else if(ev == can_event_message)
	  {
		  leds_on( LEDS_LED3 );
		  canconverter_handle_rx_data();
		  leds_off( LEDS_LED3 );
	  }

  }

  PROCESS_END();
}






