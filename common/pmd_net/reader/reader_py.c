// created 06.07.2011

#include "reader_py.h"

//TODO: do it on with macros for all simple lvl3 protocols: 'PMD_NET_WRITE_FUNK(reader, send_msg, READER_SEND_MSG)'
inline static PyObject* call_reader_write(uint8_t operation) {
	bytearr_t* arr = NULL;
	pmd_net_reader_data_t data;
	data.operation = operation;
	int rc = pmd_net_reader_write_data(arr, &data);
	if (rc)
		return Py_BuildValue("(is)", rc, NULL);
	else
		return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_reader_w_send_msg_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_SEND_MSG);
}
static PyObject* pmd_net_reader_w_green_led_on_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_GREEN_LED_ON);
}
static PyObject* pmd_net_reader_w_green_led_off_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_GREEN_LED_OFF);
}
static PyObject* pmd_net_reader_w_beep_on_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_BEEP_ON);
}
static PyObject* pmd_net_reader_w_beep_off_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_BEEP_OFF);
}

/*
static PyObject* pmd_net_reader_w_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_reader_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_reader_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
*/
static PyObject* pmd_net_reader_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_reader_data_t* data = NULL;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_reader_read_data(&arr, data);
	return Py_BuildValue("(iI)", rc, "pmd_net_reader_data", data->operation);
}

static PyMethodDef pmd_net_reader_methods[] = {
	{"send_msg", pmd_net_reader_w_send_msg_py, METH_VARARGS, "(i)pack command: the read code| args: - | return:(rc -int, packed_msg -str)"},
	{"green_led_off", pmd_net_reader_w_green_led_off_py, METH_VARARGS, "(o)pack command: turn green led off| args: - | return:(rc -int, packed_msg -str)"},
	{"green_led_on", pmd_net_reader_w_green_led_on_py, METH_VARARGS, "(o)pack command: turn green led on| args: - | return:(rc -int, packed_msg -str)"},
	{"beep_on", pmd_net_reader_w_beep_on_py, METH_VARARGS, "(o)pack command: turn beeper on| args: - | return:(rc -int, packed_msg -str)"},
	{"beep_off", pmd_net_reader_w_beep_off_py, METH_VARARGS, "(o)pack command: turn beeper off| args: - | return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_reader_w_py, METH_VARARGS, "pack command for reader, return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_reader_r_py, METH_VARARGS, "unpack command for reader| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_reader(void) {
	(void) Py_InitModule("pmd_net_reader", pmd_net_reader_methods);
}
