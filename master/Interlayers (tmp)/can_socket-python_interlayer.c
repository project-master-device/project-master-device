// 08.02.2011
// 

//TODO: licence - MIT

#include "can_socket-python_interlayer.h"

static PyObject* can_init_socket(PyObject *self, PyObject *args) {
	char* device = NULL;
	if(!PyArg_ParseTuple(args, "s", &device))
		return Py_BuildValue("i", -1);
	return Py_BuildValue("i", can_socket);
}


static PyObject* can_recieve_some_data(PyObject *self, PyObject *args) {
	unsigned int can_socket = 0;
	if(!PyArg_ParseTuple(args, "I", &can_socket))
		return Py_BuildValue("i", -1);

	int int_len = (int)frame.can_dlc;
	return Py_BuildValue("s#", frame.data, int_len);

	struct can_frame frame;
	int recieved = can_try_recieve_frame(can_socket, frame);

	if (recieved != -1) {
		return Py_BuildValue("s#", frame.data, frame.can_dlc);
	}
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject* can_py_send_frame(PyObject *self, PyObject *args) {
	unsigned int can_socket = 0;
	char* frame_data = NULL;
	unsigned int frame_can_id = 0;
	unsigned int frame_data_len_i = 0;
	if(!PyArg_ParseTuple(args, "Is#I", &can_socket, &frame_data, &frame_data_len_i, &frame_can_id)
		 || frame_data_len_i > 8) {
		return Py_BuildValue("i", -1);
	}
	int bytes_sent = can_send_frame(can_socket, frame_data, frame_data_len_i, frame_can_id);
	return Py_BuildValue("i", bytes_sent);
}


static PyMethodDef CANsocketMethods[] = {
	{"init_socket", can_py_init_socket, METH_VARARGS, "initialize can socket."},
	{"recieve_some_data", can_py_recieve_some_data, METH_VARARGS, "recieve data from recieved frame."},
	{"send_frame", can_py_send_frame, METH_VARARGS, "send frame through socket."},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initcan_socket(void) {
	(void) Py_InitModule("can_socket", CANsocketMethods);
}
