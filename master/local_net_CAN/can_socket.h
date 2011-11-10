/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
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
//#define CANSOCKET_LOG
//#define CAN_RECIEVE_ERRORS
#define CANSOCKET_CANID_PACKER
#define CAN_FRAME_INF_PACK_VALIDATE

#ifdef CANSOCKET_CANID_PACKER
#include "../../common/net/can_drv_structs.h"
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
