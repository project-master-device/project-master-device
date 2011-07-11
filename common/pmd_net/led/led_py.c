// created 06.07.2011

#include "led_py.h"

inline static PyObject* call_led_write(uint8_t operation) {
	bytearr_t arr;
	pmd_net_led_data_t data;
	data.operation = operation;
	int rc = pmd_net_led_write_data(&arr, &data);
	return pmd_net_return_arr(rc, &arr);
}
static PyObject* pmd_net_led_w_on_py(PyObject* self, PyObject* args) {
	return call_led_write(PMD_NET_LED_ON);
}
static PyObject* pmd_net_led_w_off_py(PyObject* self, PyObject* args) {
	return call_led_write(PMD_NET_LED_OFF);
}
static PyObject* pmd_net_led_w_toggle_py(PyObject* self, PyObject* args) {
	return call_led_write(PMD_NET_LED_TOGGLE);
}

/*static PyObject* pmd_net_led_w_py(PyObject* self, PyObject* args) {
	pmd_net_led_data_t data;
	if(!PyArg_ParseTuple(args, "I", &data.operation)) {
		return Py_BuildValue("(is)", rc, NULL);
	}
	return call_led_write(data.operation);
}*/

static PyObject* pmd_net_led_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_led_data_t data;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_led.read expected string");
		return Py_BuildValue("(is)", -1, NULL);
	}
	int rc = pmd_net_led_read_data(&arr, &data);
	return pmd_net_return_op(rc, data.operation);
}

static PyMethodDef pmd_net_led_methods[] = {
	{"w_on", pmd_net_led_w_on_py, METH_VARARGS, "(o)pack command: turn LED on| args: - | return:(rc -int, packed_msg -str)"},
	{"w_off", pmd_net_led_w_off_py, METH_VARARGS, "(o)pack command: turn LED off| args: - | return:(rc -int, packed_msg -str)"},
	{"w_toggle", pmd_net_led_w_toggle_py, METH_VARARGS, "(o)pack command: toggle LED state| args: - |return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_led_w_py, METH_VARARGS, "pack command for LED, return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_led_r_py, METH_VARARGS, "unpack command for LED| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_led(void) {
	(void) Py_InitModule("pmd_net_led", pmd_net_led_methods);
}
