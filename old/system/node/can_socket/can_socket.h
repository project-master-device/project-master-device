#ifndef Py_CANSOCKETMODULE_H
#define Py_CANSOCKETMODULE_H

// 9.11.2010
// 21.11.2010

//TODO: licence - MIT

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
