// created 06.07.2011

#include "system_py.h"

static PyObject* pmd_net_heartbeat_write_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	if(!PyArg_ParseTuple(args, "s#", &arr->itself, &arr->len)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_heartbeat_write_data(arr);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

/*
typedef struct {
    uint8_t operation;
    config_cnf_t * config;
    config_section_t * section;
} pmd_net_sys_config_data_t;
*/

// ["config_cnf", ("config_section", id -int, ["options", ("option", key -str, value -str/int?, type -str/int?), ...]), ...]
// ["config_cnf", ("config_section", id -int, ["options", ("option_py", type, ("option", key -str, value -str/int?)), ...]), ...]
/*
PyObject* pyObj;
if(!PyArg_ParseTuple(args,"O",&pyObj) || !pyTuple_Check(pyObj))
   return NULL;

tupleSize = PyTuple_Size(pyObj);
array = (double*) malloc(sizeof(double)*tupleSize);
for(i=0; i < tupleSize; i++ ) {
   tupleItem = PyTuple_GetItem(pyObj, i);
   if ( PyFloat_Check(tupleItem) ) {
      array[i] = PyFloat_AsDouble(tupleItem);
   } else {
      printf("Error: tuple contains a non-float value");
      exit(1);
   }
}
*/
static PyObject* pmd_net_sys_config_write_data_py(PyObject *self, PyObject *args) {
	bytearr_t* arr;
	pmd_net_sys_config_data_t* data;
	PyObject* option_py;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_sys_config_data", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_config_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_sys_config_read_data_py(PyObject *self, PyObject *args) {
	bytearr_t* arr;
	pmd_net_sys_config_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_sys_config_data_t", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_config_read_data(arr, data);
	return Py_BuildValue("(i(sI))", rc, "pmd_net_sys_config_data_t", data->operation);
}


static PyObject* pmd_net_sys_set_op_write_data_py(PyObject *self, PyObject *args) {
	bytearr_t* arr;
	pmd_net_sys_set_op_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_sys_set_op_data_t", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_set_op_write_data(arr, data);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_sys_set_op_read_data_py(PyObject *self, PyObject *args) {
	bytearr_t* arr;
	pmd_net_sys_set_op_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_sys_set_op_data_t", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_set_op_read_data(arr, data);
	return Py_BuildValue("(i(sI))", rc, "pmd_net_sys_set_op_data_t", data->operation);
}

static PyMethodDef pmd_net_system_methods[] = {
	{"heartbeat_write", pmd_net_heartbeat_write_data_py, METH_VARARGS, "make data for heartbeat msg"},
	{"config_write", pmd_net_sys_config_write_data_py, METH_VARARGS, "pack command for config"},
	{"config_read", pmd_net_sys_config_read_data_py, METH_VARARGS, "unpack command for config"},
	{"set_op_write", pmd_net_sys_set_op_write_data_py, METH_VARARGS, "pack command for set_op"},
	{"set_op_read", pmd_net_sys_set_op_read_data_py, METH_VARARGS, "unpack command for set_op"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_system(void) {
	(void) Py_InitModule("pmd_net_system", pmd_net_system_methods);
}
