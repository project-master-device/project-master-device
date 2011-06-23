#ifndef CANSOCKET_PYTHON_INTERLAYER_H
#define CANSOCKET_PYTHON_INTERLAYER_H

// 08.02.2011
// 

//TODO: licence - MIT

#include <Python.h>

#include "can_socket.h"

#define CANSOCKET_LOG
// no id mask - reads ALL incoming:
#define CANSOCKET_LOOPBACK

/**
 * init_socket - initialize non-blocking can socket
 * @ 1) device: device name to bind socket with (example: 'can0') - string
 * @return: created socket num - int (-1 if err)
 */
static PyObject* can_py_init_socket(PyObject *self, PyObject *args);

/**
 * recieve_data - recieve data from recieved frame
 * @ 1) can_socket: socket to use - unsigned int
 * @return: recieved data - 8-len string, None if frame not completed (-1 if err)
 */
static PyObject* can_py_recieve_some_data(PyObject *self, PyObject *args);

/**
 * send_frame - send frame through socket
 * @ 1) can_socket: socket to use - unsigned int
 * @ 2) frame_data: frame data - string
 * @ 3) frame_can_id: 32 bit CAN_ID+EFF/RTR/ERR flags - unsigned int
 * @return: bytes sent - int (-1 if err)
 */
static PyObject* can_py_send_frame(PyObject *self, PyObject *args);

#endif // CANSOCKET_PYTHON_INTERLAYER_H
