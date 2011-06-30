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

/**
 * @file can_socket
 * @brief CAN send/receive frame level
 */

#ifndef Py_CANSOCKETMODULE_H
#define Py_CANSOCKETMODULE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

/* defines:
		CANSOCKET_LOG - logging (with printf)
		CANSOCKET_LOOPBACK - reads ALL incoming
		CANSOCKET_CANID_PACKER
*/
#define CANSOCKET_LOOPBACK
#define CANSOCKET_LOG
//#define CAN_RECIEVE_ERRORS
#define CANSOCKET_CANID_PACKER
#define CAN_FRAME_INF_PACK_VALIDATE

#ifdef CANSOCKET_CANID_PACKER
#include "../../../common/net/can_drv_structs.h"
#endif


/**
 * init_socket - initialize can socket
 * @ 1) device: device name to bind socket with (example: "can0") - string
 * @return: can_socket: created socket num (error: -1) - int
 */
int can_cs_init_socket(const char *device);

/**
 * recieve_data - recieve data from recieved frame
 * @ 1) can_socket: socket to use - unsigned int
 * @ 2) frame: recieved frame struct can_frame from can.h
 * @return: 0 if recieved, 1 if not, (read from socket error: 2) - int
 */
int can_cs_try_receive_frame(const int can_socket, struct can_frame* frame);

/**
 * send_frame - send frame through socket
 * @ 1) can_socket: socket to use - unsigned int
 * @ 2) frame: frame to send - struct can_frame
 * @return: bytes sent (error: -1) - int
 */
int can_cs_send_frame(const int can_socket, const struct can_frame* frame);

/**
 * @struct can_service_inf_t
 * @brief Controller Area Network Identifier structure
 * for internal use in can_protocols
 * bit 0-28	: CAN identifier (11/29 bit)
 * bit 29	: error frame flag (0 = data frame, 1 = error frame)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit) (ide)
 */
int can_frame_inf_pack(uint32_t* packed, const uint32_t can_id, const uint8_t err, const uint8_t rtr, const uint8_t ide);
can_frame_inf_t can_frame_inf_unpack(const uint32_t packed);

#endif // Py_CANSOCKETMODULE_H
