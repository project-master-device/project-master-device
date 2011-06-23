// 10.02.2011
// 05.04.2011

/*
Copyright (C) 2011 by <Project Master Device>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "can_socket.h"

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef CANSOCKET_LOG

#include <stdio.h>

void info_print_frame(const struct can_frame* frame) {
	printf("    frame.can_id=%u;\n", frame->can_id);
	printf("    hex:");
	int i;
	for (i=0; i < frame->can_dlc; i++) {
		printf("__%1d_|", i);
	}
	printf("\n        ");
	for (i=0; i < frame->can_dlc; i++) {
		printf("_%2X_|", frame->data[i]);
	}
	printf("\n    frame.can_dlc=%u;\n", frame->can_dlc);
	return;
}

void info_print_thread(void) {
	printf("INFO can_socket module: PID of this process: %d\n", getpid());
	printf("INFO can_socket module: The ID of this thread is: %u\n", (unsigned int)pthread_self());
	return;
}

#endif // CANSOCKET_LOG


int can_cs_init_socket(const char *device) {
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	int can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (can_socket < 0)	return can_socket;

	strcpy(ifr.ifr_name, device);
	if ( ioctl(can_socket, SIOCGIFINDEX, &ifr) )
		return -1;
	
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	
	if ( bind(can_socket, (struct sockaddr *)&addr, sizeof(addr)) )
		return -1;
	
	// make it non-blocking:
	if ( fcntl(can_socket, F_SETFL, O_NONBLOCK) == -1 )
		return -1;

#ifdef CAN_RECIEVE_ERRORS
	can_err_mask_t err_mask = ( CAN_ERR_TX_TIMEOUT | CAN_ERR_LOSTARB | CAN_ERR_CRTL | CAN_ERR_PROT |
								CAN_ERR_TRX | CAN_ERR_ACK | CAN_ERR_BUSOFF | CAN_ERR_RESTARTED ); // CAN_ERR_BUSERROR
	if ( setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask)) )
		return -1;
#endif

#ifndef CANSOCKET_LOOPBACK
// turn off loopback sending
	int loopback = 0; /* 0 = disabled, 1 = enabled */
	if ( setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback)) )
		return -1;
#endif // CANSOCKET_LOOPBACK

	return can_socket;
}


int can_cs_try_receive_frame(const int can_socket, struct can_frame* frame) {
	static char can_incoming_buffer[16];
	static unsigned can_incoming_buffer_len = 0;

	// just full frame in buffer, nothing more:
	unsigned bytes_left = sizeof(struct can_frame) - can_incoming_buffer_len;
	int rbytes = read(can_socket, &can_incoming_buffer[can_incoming_buffer_len], bytes_left);
	if (rbytes > 0) {
		can_incoming_buffer_len += rbytes;
		if (can_incoming_buffer_len == sizeof(struct can_frame)) {
			can_incoming_buffer_len = 0;
			memcpy(frame, &can_incoming_buffer, sizeof(struct can_frame));

			#ifdef CANSOCKET_LOG
			printf("INFO can_socket module: recieved frame:\n");
			info_print_frame(frame);
			#endif // CANSOCKET_LOG

			return 0;
		}
	} else if (rbytes < 0) {
		// read from socket error
		return 2;
	}

	// 1 if full frame not recieved:
	return 1;
}


int can_cs_send_frame(const int can_socket, const struct can_frame* frame) {
#ifdef CANSOCKET_LOG
	printf("INFO can_socket module: sending frame :\n");
	info_print_frame(frame);
#endif // CANSOCKET_LOG

	return write(can_socket, frame, sizeof(*frame));
}

#ifdef CANSOCKET_CANID_PACKER

#define SERVICE_INF_CANID_OFFSET			0			// start
#define SERVICE_INF_CANID_MASK				0x7ff		// 11b
#define SERVICE_INF_CANID_MASK_EXTENDED		0xfffffff	// 28b
#define SERVICE_INF_ERR_OFFSET				29			// 29+1+1
#define SERVICE_INF_ERR_MASK				0x1			// 1b
#define SERVICE_INF_RTR_OFFSET				30			// 29+1
#define SERVICE_INF_RTR_MASK				0x1			// 1b
#define SERVICE_INF_IDE_OFFSET				31			// 29
#define SERVICE_INF_IDE_MASK				0x1			// 1b
#define SERVICE_INF_MASK					0x3fff		// 14b
#define SERVICE_INF_MASK_EXTENDED			0xffffffff	// 32b

int can_frame_inf_pack(uint32_t* packed, const uint32_t can_id, const uint8_t err, const uint8_t rtr, const uint8_t ide) {
#ifdef CAN_FRAME_INF_PACK_VALIDATE
	uint32_t canid_mask = SERVICE_INF_CANID_MASK;
	if (ide) {
		canid_mask = SERVICE_INF_CANID_MASK_EXTENDED;
	}
	if (err > SERVICE_INF_ERR_MASK) return 1;
	if (rtr > SERVICE_INF_RTR_MASK) return 1;
	if (ide > SERVICE_INF_IDE_MASK) return 1;
	if (can_id > canid_mask) return 1;
#endif
	uint32_t canid_b = can_id << SERVICE_INF_CANID_OFFSET;
	uint32_t err_b = err << SERVICE_INF_ERR_OFFSET;
	uint32_t rtr_b = rtr << SERVICE_INF_RTR_OFFSET;
	uint32_t ide_b = ide << SERVICE_INF_IDE_OFFSET;
	*packed = canid_b | err_b | rtr_b | ide_b;
	return 0;
}
can_frame_inf_t can_frame_inf_unpack(const uint32_t packed) {
	can_frame_inf_t unpacked;
	unpacked.ide = (packed >> SERVICE_INF_IDE_OFFSET) & SERVICE_INF_IDE_MASK;
	unpacked.rtr = (packed >> SERVICE_INF_RTR_OFFSET) & SERVICE_INF_RTR_MASK;
	unpacked.err = (packed >> SERVICE_INF_ERR_OFFSET) & SERVICE_INF_ERR_MASK;
	uint32_t canid_mask = SERVICE_INF_CANID_MASK;
	if (unpacked.ide) {
		canid_mask = SERVICE_INF_CANID_MASK_EXTENDED;
	}
	unpacked.id = (packed >> SERVICE_INF_CANID_OFFSET) & canid_mask;
	return unpacked;
}

#endif
