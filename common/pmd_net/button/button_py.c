// created 06.07.2011

#include "button_py.h"

inline static PyObject* call_button_write(uint8_t operation) {
	bytearr_t arr;
	pmd_net_button_data_t data;
	data.operation = operation;
	int rc = pmd_net_button_write_data(&arr, &data);
	return pmd_net_return_arr(rc, &arr);
}

static PyObject* pmd_net_button_w_down_py(PyObject* self, PyObject* args) {
	return call_button_write(PMD_NET_BUTTON_DOWN);
}

static PyObject* pmd_net_button_w_up_py(PyObject* self, PyObject* args) {
	return call_button_write(PMD_NET_BUTTON_UP);
}

/*
static PyObject* pmd_net_button_w_py(PyObject* self, PyObject* args) {
	pmd_net_button_data_t data;
	if(!PyArg_ParseTuple(args, "I", &data.operation)) {
		return Py_BuildValue("(is)", rc, NULL);
	}
	return call_button_write(data.operation);
}
*/
static PyObject* pmd_net_button_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_button_data_t data;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_button.read expected string");
		return Py_BuildValue("(is)", -1, NULL);
	}
	int rc = pmd_net_button_read_data(&arr, &data);
	return pmd_net_return_op(rc, data.operation);
}

static PyMethodDef pmd_net_button_methods[] = {
	{"w_down", pmd_net_button_w_down_py, METH_VARARGS, "(i)pack command: button down| args: - | return:(rc -int, packed_msg -str)"},
	{"w_up", pmd_net_button_w_up_py, METH_VARARGS, "(i)pack command: button up| args: - | return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_button_w_py, METH_VARARGS, "pack command for button| args: ... | return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_button_r_py, METH_VARARGS, "unpack command for button| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_button(void) {
	(void) Py_InitModule("pmd_net_button", pmd_net_button_methods);
}
