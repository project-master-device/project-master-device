/*
 * can_net_sync_wrapper.h
 *
 *  Created on: Jun 29, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#ifndef CAN_NET_SYNC_PY_H
#define CAN_NET_SYNC_PY_H

#include <Python.h>
#include "can_net_sync_wrapper.h"

/**
 * can_net_sync_py_init - initialize, start listening for incoming messages
 * @ 1) port: port - uint8_t
 * @ 2) msgs_limit: msgs limit - int
 * @ 3) timeout_cycles: timeout cycles - int
 * @ 4) send_frame_timeout_us: look at can_net::can_net_init() - const uint32_t
 * @ 5) confirmation_tics: look at can_net::can_net_init() - const uint32_t
 * @return: return codes: look at can_net::can_net_init() - int
 */
static PyObject* can_net_sync_py_init(PyObject *self, PyObject *args);

/**
 * can_net_sync_py_send - send message
 * @ 1) msg: message to send - msg_lvl2_t
 * @return: return codes: look at callback return codes in can_net_middle.h
 * msg_lvl2_t in PYTHON: ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
 */
static PyObject* can_net_sync_py_send(PyObject *self, PyObject *args);

/**
 * can_net_sync_py_recv - recv next message
 * @ 1) port: port - uint8_t
 * @return: tuple: (msg: received message - msg_lvl2_t, return code: 0 - ok, 1 - wrong port, 2 - timeout)
 * tries to take from port buffer, if nothing there - waits until at least one appears in buffer or timeout
 * msg_lvl2_t in PYTHON: ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
 */
static PyObject* can_net_sync_py_recv(PyObject *self, PyObject *args);

/**
 * can_net_sync_py_close - close, stop listening for incoming messages
 * @ 1) port: port - uint8_t
 * @return: return codes: 0: ok, else: smth wrong happend
 * if called while other (recv, send, init) funcs are working they will segfault (most probably)
 */
static PyObject* can_net_sync_py_close(PyObject *self, PyObject *args);

#endif // CAN_NET_SYNC_PY_H
