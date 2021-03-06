/*
 * Copyright (c) 2010, Mariano Alvira <mar@devl.org> and other contributors
 * to the MC1322x project (http://mc1322x.devl.org) and Contiki.
 *
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
 * This file is part of the Contiki OS.
 *
 * $Id: contiki-maca.c,v 1.4 2010/09/13 14:15:58 maralvira Exp $
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* contiki */
#include "radio.h"
#include "sys/process.h"
#include "net/packetbuf.h"
#include "net/netstack.h"

#include "mc1322x.h"
#include "contiki-conf.h"

#define CONTIKI_MACA_DEBUG 0
#if CONTIKI_MACA_DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#ifndef MACA_RAW_PREPEND
#define MACA_RAW_PREPEND 0xff
#endif

#ifndef BLOCKING_TX
#define BLOCKING_TX 1
#endif

static volatile uint8_t tx_complete;

/* contiki mac driver */

int contiki_maca_init(void);
int contiki_maca_on_request(void);
int contiki_maca_off_request(void);
int contiki_maca_read(void *buf, unsigned short bufsize);
int contiki_maca_prepare(const void *payload, unsigned short payload_len);
int contiki_maca_transmit(unsigned short transmit_len);
int contiki_maca_send(const void *payload, unsigned short payload_len);
int contiki_maca_channel_clear(void);
int contiki_maca_receiving_packet(void);
int contiki_maca_pending_packet(void);

const struct radio_driver contiki_maca_driver =
{
	.init = contiki_maca_init,
	.prepare = contiki_maca_prepare,
	.transmit = contiki_maca_transmit,
	.send = contiki_maca_send,
	.read = contiki_maca_read,
	.receiving_packet = contiki_maca_receiving_packet,
	.pending_packet = contiki_maca_pending_packet,
	.channel_clear = contiki_maca_channel_clear,
	.on = contiki_maca_on_request,
	.off = contiki_maca_off_request,
};

static volatile uint8_t contiki_maca_request_on = 0;
static volatile uint8_t contiki_maca_request_off = 0;

static process_event_t event_data_ready;

volatile packet_t *prepped_p;

int contiki_maca_init(void) {
	prepped_p = 0;
	trim_xtal();
	vreg_init();
	contiki_maca_init();
	set_channel(0); /* channel 11 */
	set_power(0x12); /* 0x12 is the highest, not documented */
	return 1;
}

/* CCA not implemented */
int contiki_maca_channel_clear(void) {
	return 1;
}

/* not sure how to check if a reception is in progress */
int contiki_maca_receiving_packet(void) {
	return 0;
}

int contiki_maca_pending_packet(void) {
	if (rx_head != NULL) {
		return 1;
	} else {
		return 0;
	}
}

int contiki_maca_on_request(void) {
	contiki_maca_request_on = 1;
	contiki_maca_request_off = 0;
	return 1;
}

int contiki_maca_off_request(void) {
	contiki_maca_request_on = 0;
	contiki_maca_request_off = 1;
	return 1;
}

/* it appears that the mc1332x radio cannot */
/* receive packets where the last three bits of the first byte */
/* is equal to 2 --- even in promiscuous mode */
int contiki_maca_read(void *buf, unsigned short bufsize) {
	volatile uint32_t i;
	volatile packet_t *p;
	
	if((p = rx_packet())) {
		PRINTF("maca read");
#if CONTIKI_MACA_RAW_MODE
		/* offset + 1 and size - 1 to strip the raw mode prepended byte */
		/* work around since maca can't receive acks bigger than five bytes */
		PRINTF(" raw mode");
		p->length -= 1;
		p->offset += 1;
#endif
		PRINTF(": p->length 0x%0x bufsize 0x%0x \n\r", p->length, bufsize);
		if((p->length) < bufsize) bufsize = (p->length);
		memcpy(buf, (uint8_t *)(p->data + p->offset), bufsize);
#if CONTIKI_MACA_DEBUG
		for( i = p->offset ; i < (bufsize + p->offset) ; i++) {
			PRINTF(" %02x",p->data[i]);
		}
#endif 
		PRINTF("\n\r");
		free_packet(p);
		return bufsize;
	} else {
		return 0;
	}
}

int contiki_maca_prepare(const void *payload, unsigned short payload_len) {
	volatile int i;
	volatile packet_t *p;

	if ((prepped_p == 0) 
	    && (p = get_free_packet())) {
		PRINTF("contiki maca prepare");
#if CONTIKI_MACA_RAW_MODE
		p->offset = 1;
		p->length = payload_len + 1;
#else 
		p->offset = 0;
		p->length = payload_len;
#endif
		if(payload_len > MAX_PACKET_SIZE)  return RADIO_TX_ERR;
		memcpy((uint8_t *)(p->data + p->offset), payload, payload_len);
#if CONTIKI_MACA_RAW_MODE
		p->offset = 0;
		p->data[0] = CONTIKI_MACA_PREPEND_BYTE;
		PRINTF(" raw mode");
#endif
#if CONTIKI_MACA_DEBUG
		PRINTF(": sending %d bytes\n\r", payload_len);
		for(i = p->offset ; i < (p->length + p->offset); i++) {
			PRINTF(" %02x",p->data[i]);
		}
		PRINTF("\n\r");
#endif

		prepped_p = p;

		return RADIO_TX_OK;
	} else {
		PRINTF("couldn't get free packet for contiki_maca_send\n\r");
		return RADIO_TX_ERR;
	}
}

int contiki_maca_transmit(unsigned short transmit_len) {
	PRINTF("contiki maca transmit\n\r");
#if BLOCKING_TX
	tx_complete = 0;
#endif
	tx_packet(prepped_p);
	prepped_p = 0;
#if BLOCKING_TX
	/* block until tx_complete, set by contiki_maca_tx_callback */
	/* there are many places in contiki that rely on the */
	/* transmit call to block */
 	while(!tx_complete && (tx_head != 0));
#endif	
}

int contiki_maca_send(const void *payload, unsigned short payload_len) {
	contiki_maca_prepare(payload, payload_len);
	contiki_maca_transmit(payload_len);
	return RADIO_TX_OK;
}

PROCESS(contiki_maca_process, "maca process");
PROCESS_THREAD(contiki_maca_process, ev, data)
{
 	volatile uint32_t i;
	int len;
	
 	PROCESS_BEGIN();

	while (1) {
		PROCESS_PAUSE();

		/* check if there is a request to turn the radio on or off */
		if(contiki_maca_request_on == 1) {
			contiki_maca_request_on = 0;
			maca_on();
 		}

		if(contiki_maca_request_off == 1) {
			contiki_maca_request_off = 0;
			maca_off();
 		}

		if (rx_head != NULL) {
			packetbuf_clear();
			len = contiki_maca_read(packetbuf_dataptr(), PACKETBUF_SIZE);
			if(len > 0) {
				packetbuf_set_datalen(len);				
				NETSTACK_RDC.input();
			}
		}
		
 	};
	
 	PROCESS_END();
}

#if BLOCKING_TX
void maca_tx_callback(volatile packet_t *p __attribute((unused))) {
	tx_complete = 1;
}
#endif
