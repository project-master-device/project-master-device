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


#include "can_net_sync_py.h"

typedef uint32_t python_uint_t;

static PyObject* can_net_sync_py_init(PyObject* self, PyObject* args) {
	unsigned port;
	int msgs_limit;
	int timeout_cycles;
	unsigned send_frame_timeout_us;
	unsigned confirmation_tics;
	if(!PyArg_ParseTuple(args, "IiiII", &port, &msgs_limit, &timeout_cycles, &send_frame_timeout_us, &confirmation_tics)) {
		PyErr_Format(PyExc_TypeError, "can_net_sync.init - expected uint,int,int,uint,uint");
		return Py_BuildValue("i", -1);
	}
	int rc = can_net_sync_init(port, msgs_limit, timeout_cycles, send_frame_timeout_us, confirmation_tics);
	return Py_BuildValue("i", rc);
}

static PyObject* can_net_sync_py_send(PyObject* self, PyObject* args) {
	msg_lvl2_t msg;
	char* msg_lvl2_name;
	char* msg_metadata_name;

    python_uint_t hw_addr_buf;
    python_uint_t port_buf;
    python_uint_t is_system_buf;
	python_uint_t id_buf;
	python_uint_t data_len_buf;

	// ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
	if(!PyArg_ParseTuple(args, "(s(sIIII)s#)", &msg_lvl2_name, &msg_metadata_name,
			&hw_addr_buf, &port_buf, &is_system_buf, &id_buf, &msg.data.itself, &data_len_buf)) {
		PyErr_Format(PyExc_TypeError, "can_net_sync.send - expected ('',('',uint,uint,uint),str)");
		return Py_BuildValue("i", -1);
	}
    msg.meta.hw_addr = (uint32_t)hw_addr_buf;
    msg.meta.port = (uint8_t)port_buf;
    msg.meta.is_system = (uint8_t)is_system_buf;
	msg.meta.id = (uint16_t)id_buf;
	msg.data.len = (uint32_t)data_len_buf; // ???

	int rc = can_net_sync_send(&msg);
	return Py_BuildValue("i", rc);
}

static PyObject* can_net_sync_py_recv(PyObject* self, PyObject* args) {
	unsigned port;
	if(!PyArg_ParseTuple(args, "I", &port)) {
		PyErr_Format(PyExc_TypeError, "can_net_sync.recv - expected uint");
		return Py_BuildValue("(is)", -1, NULL);
	}
	msg_lvl2_t* msg;
	int rc = can_net_sync_recv(port, &msg);
	if (rc) {
		return Py_BuildValue("(is)", rc, NULL);
	} else {
		// ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
		return Py_BuildValue("(i(s(sIIII)s#))", rc, "msg_lvl2", "msg_metadata",
				msg->meta.hw_addr, msg->meta.port, msg->meta.is_system, msg->meta.id, msg->data.itself, msg->data.len);
	}
}
/*
static PyObject* can_net_sync_py_close(PyObject *self, PyObject *args) {
	unsigned port;
	if(!PyArg_ParseTuple(args, "I", &port)) {
		PyErr_Format(PyExc_TypeError, "can_net_sync.close - expected uint");
		return Py_BuildValue("i", -1);
	}

	int rc = can_net_sync_close(port);
	return Py_BuildValue("i", rc);
}
*/

/* ----------------------------------------PY_MODULE--------------------------------------------*/

static PyMethodDef can_net_sync_methods[] = {
	{"init", can_net_sync_py_init, METH_VARARGS, "initialize, start listening for incoming messages | args: msgs_limit -int, timeout_cycles -int, send_frame_timeout_us -int, confirmation_tics -int | return: rc -int"},
	{"send", can_net_sync_py_send, METH_VARARGS, "send message | args: ('msg_lvl2', ('msg_metadata', hw_addr -int, port -int, is_system -int, id - int), data -string)| return: rc -int"},
	{"recv", can_net_sync_py_recv, METH_VARARGS, "recv next message | args: port - int | return: rc -int"},
//	{"close", can_net_sync_py_close, METH_VARARGS, "close port, stop listening for incoming messages | args: port - int | return: rc -int"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initcan_net_sync(void) {
	(void) Py_InitModule("can_net_sync", can_net_sync_methods);

	PyObject * rc_ok = Py_BuildValue("i", 0);
	PyModule_AddObject(m, "rc_ok", rc_ok);

	PyObject * send_rc_candrv_timer_err = Py_BuildValue("i", CAN_DRV_RC_TIMER_ERR);
	PyModule_AddObject(m, "send_rc_candrv_timer_err", send_rc_candrv_timer_err);

	PyObject * send_rc_can_drv_norm = Py_BuildValue("i", CAN_DRV_RC_TIMEOUT);
	PyModule_AddObject(m, "send_rc_candrv_timeout", send_rc_candrv_timeout);

	PyObject * send_rc_can_drv_norm = Py_BuildValue("i", CAN_DRV_RC_NORM);
	PyModule_AddObject(m, "send_rc_candrv_norm", send_rc_can_drv_norm);

	PyObject * send_rc_invalid_port = Py_BuildValue("i", CAN_NET_SYNC_SEND_RC_INVALID_PORT);
	PyModule_AddObject(m, "send_rc_invalid_port", send_rc_invalid_port);

	PyObject * recv_rc_timeout = Py_BuildValue("i", CAN_NET_SYNC_RECV_RC_TIMEOUT);
	PyModule_AddObject(m, "recv_rc_timeout", recv_rc_timeout);
}
