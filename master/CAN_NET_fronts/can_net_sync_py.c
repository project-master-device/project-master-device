/*
 * can_net_sync_wrapper.h
 *
 *  Created on: Jun 29, 2011
 *      Author: Gennady.Kalashnikov
 */

//TODO: licence - MIT

#include "can_net_sync_py.h"

static PyObject* can_net_sync_py_init(PyObject* self, PyObject* args) {
	unsigned port;
	int msgs_limit;
	int timeout_cycles;
	unsigned send_frame_timeout_us;
	unsigned confirmation_tics;
	if(!PyArg_ParseTuple(args, "IiiII", &port, &msgs_limit, &timeout_cycles, &send_frame_timeout_us, &confirmation_tics)) {
		return Py_BuildValue("i", -1);
	}
	int rc = can_net_sync_init(port, msgs_limit, timeout_cycles, send_frame_timeout_us, confirmation_tics);
	return Py_BuildValue("i", rc);
}

static PyObject* can_net_sync_py_send(PyObject* self, PyObject* args) {
	msg_lvl2_t msg;
	int data_len;
	char* msg_lvl2_name;
	char* msg_metadata_name;

	// ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
	if(!PyArg_ParseTuple(args, "(s(sIIII)s#)", &msg_lvl2_name, &msg_metadata_name,
			&msg.meta.hw_addr, &msg.meta.port, &msg.meta.is_system, &msg.meta.id, &msg.data.itself, &data_len)) {
		return Py_BuildValue("i", -1);
	}
	msg.data.len = data_len; // ???

	int rc = can_net_sync_send(&msg);
	return Py_BuildValue("i", rc);
}

static PyObject* can_net_sync_py_recv(PyObject* self, PyObject* args) {
	unsigned port;
	if(!PyArg_ParseTuple(args, "I", &port))
		return Py_BuildValue("i", -1);

	msg_lvl2_t* msg;
	int rc = can_net_sync_recv(port, &msg);

	// ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
	return Py_BuildValue("((s(sIIII)s#)i)", "msg_lvl2", "msg_metadata",
			msg->meta.hw_addr, msg->meta.port, msg->meta.is_system, msg->meta.id, msg->data.itself, msg->data.len, rc);
}

static PyObject* can_net_sync_py_close(PyObject *self, PyObject *args) {
	unsigned port;
	if(!PyArg_ParseTuple(args, "I", &port))
		return Py_BuildValue("i", -1);

	int rc = can_net_sync_close(port);
	return Py_BuildValue("i", rc);
}

static PyMethodDef can_net_sync_methods[] = {
	{"init", can_net_sync_py_init, METH_VARARGS, "initialize, start listening for incoming messages.(int msgs_limit, int timeout_cycles, int send_frame_timeout_us, int confirmation_tics)."},
	{"send", can_net_sync_py_send, METH_VARARGS, "send message.(msg_tuple msg)."},
	{"recv", can_net_sync_py_recv, METH_VARARGS, "recv next message.(port)."},
//	{"close", can_net_sync_py_close, METH_VARARGS, "close, stop listening for incoming messages."},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initcan_net_sync(void) {
	(void) Py_InitModule("can_net_sync", can_net_sync_methods);
}
