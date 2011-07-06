// created 06.07.2011

#include "reader_py.h"

static PyObject* pmd_net_reader_write_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_reader_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_reader_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_reader_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

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
	{"write", pmd_net_reader_write_data_py, METH_VARARGS, "pack command for reader"},
	{"read", pmd_net_reader_read_data_py, METH_VARARGS, "unpack command for reader"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_reader(void) {
	(void) Py_InitModule("pmd_net_reader", pmd_net_reader_methods);
}
