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


#ifndef Py_CANSOCKETMODULE_H
#define Py_CANSOCKETMODULE_H

#include <Python.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define CANSOCKET_LOG
// no id mask - reads ALL incoming:
#define CANSOCKET_LOOPBACK

/**
 * init_socket - initialize non-blocking can socket
 * @ 1) device: device name to bind socket with (example: 'can0') - string
 * @return: created socket num - int (-1 if err)
 */
static PyObject* can_init_socket(PyObject *self, PyObject *args);

/**
 * recieve_data - recieve data from recieved frame
 * @ 1) can_socket: socket to use - unsigned int
 * @return: recieved data - 8-len string, None if frame not completed (-1 if err)
 */
static PyObject* can_recieve_some_data(PyObject *self, PyObject *args);

/**
 * send_frame - send frame through socket
 * @ 1) can_socket: socket to use - unsigned int
 * @ 2) frame_data: frame data - string
 * @ 3) frame_can_id: 32 bit CAN_ID+EFF/RTR/ERR flags - unsigned int
 * @return: bytes sent - int (-1 if err)
 */
static PyObject* can_send_frame(PyObject *self, PyObject *args);

#endif // Py_CANSOCKETMODULE_H
