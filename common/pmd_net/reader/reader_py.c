// created 06.07.2011

#include "reader_py.h"
#define PMD_NET_READER_SEND_MSG         0
#define PMD_NET_READER_GREEN_LED_ON     1
#define PMD_NET_READER_GREEN_LED_OFF    2
#define PMD_NET_READER_BEEP_ON          3
#define PMD_NET_READER_BEEP_OFF         4

//TODO: do it on with macros: 'PMD_NET_WRITE_FUNK(reader, send_msg, READER_SEND_MSG)'
static PyObject* pmd_net_reader_write_send_msg_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t data;
	data->operation = PMD_NET_READER_SEND_MSG;
	int rc = pmd_net_reader_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
static PyObject* pmd_net_reader_write_green_led_on_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t data;
	data->operation = PMD_NET_READER_GREEN_LED_ON_MSG;
	int rc = pmd_net_reader_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
static PyObject* pmd_net_reader_write_green_led_off_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t data;
	data->operation = PMD_NET_READER_GREEN_LED_OFF_MSG;
	int rc = pmd_net_reader_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
static PyObject* pmd_net_reader_write_beep_on_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t data;
	data->operation = PMD_NET_READER_BEEP_ON_MSG;
	int rc = pmd_net_reader_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
static PyObject* pmd_net_reader_write_beep_off_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t data;
	data->operation = PMD_NET_READER_BEEP_OFF_MSG;
	int rc = pmd_net_reader_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

/*
static PyObject* pmd_net_reader_write_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_reader_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_reader_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
*/
static PyObject* pmd_net_reader_read_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_reader_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_reader_read_data(arr, data);
	return Py_BuildValue("(i(sI))", rc, "pmd_net_reader_data", data->operation);
}

static PyMethodDef pmd_net_reader_methods[] = {
	{"send_msg", pmd_net_reader_write_send_msg_data_py, METH_VARARGS, "(i)pack command: the read code, return:(rc -int, packed_msg -str)"},
	{"green_led_off", pmd_net_reader_write_green_led_off_data_py, METH_VARARGS, "(o)pack command: turn green led off, return:(rc -int, packed_msg -str)"},
	{"green_led_on", pmd_net_reader_write_green_led_on_py, METH_VARARGS, "(o)pack command: turn green led on, return:(rc -int, packed_msg -str)"},
	{"beep_on", pmd_net_reader_write_beep_on_py, METH_VARARGS, "(o)pack command: turn beeper on, return:(rc -int, packed_msg -str)"},
	{"beep_off", pmd_net_reader_write_beep_off_py, METH_VARARGS, "(o)pack command: turn beeper off, return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_reader_write_data_py, METH_VARARGS, "pack command for reader, return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_reader_read_data_py, METH_VARARGS, "unpack command for reader, return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_reader(void) {
	(void) Py_InitModule("pmd_net_reader", pmd_net_reader_methods);
}
