// created 06.07.2011

#include "led_py.h"

static PyObject* pmd_net_led_write_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_led_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_led_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_led_read_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_led_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_led_read_data(arr, data);
	return Py_BuildValue("(i(sI))", rc, "pmd_net_led_data", data->operation);
}

static PyMethodDef pmd_net_led_methods[] = {
	{"write", pmd_net_led_write_data_py, METH_VARARGS, "pack command for LED"},
	{"read", pmd_net_led_read_data_py, METH_VARARGS, "unpack command for LED"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_led(void) {
	(void) Py_InitModule("pmd_net_led", pmd_net_led_methods);
}
