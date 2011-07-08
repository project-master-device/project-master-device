// created 06.07.2011

#include "led_py.h"

static PyObject* pmd_net_led_write_on_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t data = { PMD_NET_LED_ON };
	int rc = pmd_net_led_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_led_write_off_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t data = { PMD_NET_LED_OFF };
	int rc = pmd_net_led_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_led_write_toggle_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t data = { PMD_NET_LED_TOGGLE };
	int rc = pmd_net_led_write_data(arr, &data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

/*static PyObject* pmd_net_led_write_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_led_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_led_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}*/

static PyObject* pmd_net_led_read_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_led_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_led_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_led_read_data(arr, data);
	return Py_BuildValue("(iI)", rc, data->operation);
}

static PyMethodDef pmd_net_led_methods[] = {
	{"on", pmd_net_led_write_on_py, METH_VARARGS, "(o)pack command: turn LED on, return:(rc -int, packed_msg -str)"},
	{"off", pmd_net_led_write_off_py, METH_VARARGS, "(o)pack command: turn LED off, return:(rc -int, packed_msg -str)"},
	{"toggle", pmd_net_led_write_toggle_py, METH_VARARGS, "(o)pack command: toggle LED state, return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_led_write_data_py, METH_VARARGS, "pack command for LED, return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_led_read_data_py, METH_VARARGS, "unpack command for LED, return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_led(void) {
	(void) Py_InitModule("pmd_net_led", pmd_net_led_methods);
}
