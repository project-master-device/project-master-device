// created 06.07.2011

#include "system_py.h"

/* ----------------------------------------	HEARTBEAT--------------------------------------------*/

static PyObject* pmd_net_heartbeat_w_data_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	if(!PyArg_ParseTuple(args, "s#", &arr->itself, &arr->len)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_heartbeat_write_data(arr);
	return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

/* ----------------------------------------CONFIG-----------------------------------------------*/

// ["config_cnf", ("config_section", id -int, ["options", ("option", key -str, value -str/int?, type -str/int?), ...]), ...]
// type: [0;255]
int convert_section_from_py(PyObject *section_py, config_section_t* section) {
	config_section_t section_tmp;
	PyObject* options_list_py;
	config_option_t option_tmp;
	PyObject* option_py;
	PyObject* value_py;
	char* value_char;
	config_int_t value_int;
	config_uint_t value_uint;
	int rc;

	// parse config_section_py: python tuple (str - "typename", int - id, python list - options):
	if( !PyArg_ParseTuple(section_py, "(sIO)", "config_section", &section_tmp.id, &options_list_py) || !PyList_Check(options_list_py) ) {
		PyErr_Format(PyExc_TypeError, "config_write:cnf:section expected (str,uint,PyObject)");
		return 1;
	}
	config_section_construct(section, section_tmp.id);

	// parse options_list_py: python list of tuples - options:
//	type_name = PyList_GetItem(options_list_py, 0); == "options"
	for (i = 1; i < PyList_GET_SIZE(options_list_py); i++) {
		option_py = PyList_GetItem(options_list_py, i);
		// parse option_py: python tuple (str - "typename", str - key, ? - value, int - type):
		if(!PyArg_ParseTuple(option_py, "(ssOI)", "option", &option_tmp.key, &value_py, &option_tmp.type)) {
			PyErr_Format(PyExc_TypeError, "config_write:cnf:section:option expected (str,str,PyObject,uint)");
			rc = -1; //TOBEFIXED
		} else {
			switch (option_tmp.type) {
				case 's':
					if(!PyArg_ParseTuple(value_py, "s", &value_char)) {
						PyErr_Format(PyExc_TypeError, "config_write:cnf:section:option:value expected str");
						rc = -1; //TOBEFIXED
					} else {
						rc = config_section_set_str(section, &option_tmp.key, value_char);
					}
					break;
				case 'i':
					if(!PyArg_ParseTuple(value_py, "i", &value_int)) {
						PyErr_Format(PyExc_TypeError, "config_write:cnf:section:option:value expected int");
						rc = -1; //TOBEFIXED
					} else {
						rc = config_section_set_int(section, &option_tmp.key, value_int);
					}
					break;
				case 'I':
					if(!PyArg_ParseTuple(value_py, "I", &value_uint)) {
						PyErr_Format(PyExc_TypeError, "config_write:cnf:section:option:value expected uint");
						rc = -1; //TOBEFIXED
					} else {
						rc = config_section_set_uint(section, &option_tmp.key, value_uint);
					}
					break;
			}
		}
		if (rc < 0) { //TOBEFIXED
			config_section_destruct(section);
			return 1;
		}
	}
	return 0;
}
int convert_cnf_from_py(PyObject *cnf_py, config_cnf_t* cnf) {
	// parse config_cnf_py: python list of tuples - config sections:
	if(!PyList_Check(cnf_py)) {
		PyErr_Format(PyExc_TypeError, "config_write:cnf expected list, got %s", cnf_py->ob_type->tp_name);
		return 1;
	}
	config_cnf_construct(cnf);

	int i;
	PyObject* section_py;
	config_section_t* section;
	int rc;
//	PyObject* type_name;
//	type_name = PyList_GetItem(config_cnf_py, 0); == "config_cnf"
	for (i = 1; i < PyList_GET_SIZE(cnf_py); i++) {
		section_py = PyList_GetItem(cnf_py, i);
		rc = convert_section_from_py(section_py, section);
		if (!rc)
			rc = config_cnf_add_section(cnf, section);

		if (rc) {
			config_cnf_destruct(cnf);
			return 1;
		}
	}
	return 0;
}

inline static PyObject* call_config_write_py(pmd_net_sys_config_data_t* data) {
	bytearr_t arr;
	if (pmd_net_sys_config_write_data(arr, &data))
		return Py_BuildValue("i", -1);
	else
		return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

static PyObject* pmd_net_sys_config_w_config_request_py(PyObject* self, PyObject* args) {
	pmd_net_sys_config_data_t data;
	data.operation = PMD_NET_SYS_CONFIG_REQUEST;
	return call_config_write_py(&data);
}

static PyObject* pmd_net_sys_config_w_config_full_py(PyObject* self, PyObject* args) {
		pmd_net_sys_config_data_t data;
		data.section = NULL;
		data.operation = PMD_NET_SYS_CONFIG_FULL;
		PyObject* cnf_py;
		if(!PyArg_ParseTuple(args, "O", &cnf_py)) {
			return Py_BuildValue("i", -1);
		}

		int rc = convert_cnf_from_py(cnf_py, data.config);
		if (rc) {
			return Py_BuildValue("i", -1);
		}

		return call_config_write_py(&data);
}

inline static PyObject* call_config_write_with_section(PyObject* arg, uint8_t operation) {
	pmd_net_sys_config_data_t data;
	data.operation = operation;
	data.config = NULL;
	PyObject* section_py;
	if(!PyArg_ParseTuple(arg, "O", &section_py)) {
		return Py_BuildValue("i", -1);
	}

	if ( convert_section_from_py(section_py, data.section) ) {
		return Py_BuildValue("i", -1);
	}

	return call_config_write_py(&data);
}

static PyObject* pmd_net_sys_config_w_config_section_add_py(PyObject* self, PyObject* args) {
	return call_config_write_with_section(args, PMD_NET_SYS_CONFIG_SECTION_ADD);
}

static PyObject* pmd_net_sys_config_w_config_section_del_py(PyObject* self, PyObject* args) {
	bytearr_t* arr = NULL;
	return call_config_write_with_section(args, PMD_NET_SYS_CONFIG_SECTION_ADD);
}

static PyObject* pmd_net_sys_config_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_sys_config_data_t* data = NULL;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_config_read_data(&arr, data);
	// TODO ...
	return Py_BuildValue("(iI)", rc, data->operation);
}

/* ----------------------------------------SET_OP-----------------------------------------------*/

inline static PyObject* call_setop_write(uint8_t operation) {
	bytearr_t* arr = NULL;
	pmd_net_sys_set_op_data_t data;
	data.operation = operation;
	int rc = pmd_net_system_set_op_write_data(arr, &data);
	if (rc)
		return Py_BuildValue("(is)", rc, NULL);
	else
		return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
static PyObject* pmd_net_setop_w_init_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYSTEM_SET_OP_INIT);
}
static PyObject* pmd_net_setop_w_normal_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYSTEM_SET_OP_NORMAL);
}
static PyObject* pmd_net_setop_w_configuration_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYSTEM_SET_OP_CONFIGURATION);
}

/*
static PyObject* pmd_net_sys_setop_w_py(PyObject* self, PyObject* args) {
	bytearr_t* arr;
	pmd_net_sys_set_op_data_t* data;
	if(!PyArg_ParseTuple(args, "s#(sI)", &arr->itself, &arr->len, "pmd_net_sys_set_op_data_t", &data->operation)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_set_op_write_data(arr, data);
	if (rc)
		return Py_BuildValue("(is)", rc, NULL);
	else
		return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}
*/

static PyObject* pmd_net_sys_setop_r_py(PyObject *self, PyObject *args) {
	bytearr_t* arr;
	pmd_net_sys_set_op_data_t* data;
	if(!PyArg_ParseTuple(args, "s#", &arr->itself, &arr->len)) {
		return Py_BuildValue("i", -1);
	}
	int rc = pmd_net_sys_set_op_read_data(arr, data);
	return Py_BuildValue("(iI)", rcdata->operation);
}

/* ----------------------------------------PY_MODULE--------------------------------------------*/

static PyMethodDef pmd_net_system_methods[] = {
	{"heartbeat_write", pmd_net_heartbeat_w_data_py, METH_VARARGS, "(i)make data for heartbeat msg, return:(rc -int, packed_msg -str)"},
	{"config_request", pmd_net_sys_config_w_config_request_py, METH_VARARGS, "(o?)pack command for config: request full config| args: - | return:(rc -int, packed_msg -str)"},
	{"config_full", pmd_net_sys_config_w_config_full_py, METH_VARARGS, "(i?)pack command for config: write full config| args: config -list| return:(rc -int, packed_msg -str)"},
	{"config_section_add", pmd_net_sys_config_w_section_add_py, METH_VARARGS, "(o)pack command for config: add config section| args: section -tuple| return:(rc -int, packed_msg -str)"},
	{"config_section_del", pmd_net_sys_config_w_section_del_py, METH_VARARGS, "(o)pack command for config: del config section| args: section -tuple| return:(rc -int, packed_msg -str)"},
//	{"config_write", pmd_net_sys_config_w_py, METH_VARARGS, "pack command for config, return:(rc -int, packed_msg -str)"},
	{"config_read", pmd_net_sys_config_r_py, METH_VARARGS, "unpack command for config| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{"setop_init", pmd_net_setop_w_init_py, METH_VARARGS, "pack command: set mode <ititialization> | args: - | return:(rc -int, packed_msg -str)"},
	{"setop_normal", pmd_net_setop_w_normal_py, METH_VARARGS, "pack command: set mode <normal> | args: - | return:(rc -int, packed_msg -str)"},
	{"setop_configuration", pmd_net_setop_w_configuration_py, METH_VARARGS, "pack command: set mode <configuration> | args: - | return:(rc -int, packed_msg -str)"},
//	{"setop_write", pmd_net_sys_setop_w_py, METH_VARARGS, "pack command for set_op, return:(rc -int, packed_msg -str)"},
	{"setop_read", pmd_net_sys_setop_r_py, METH_VARARGS, "unpack command for set_op| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_system(void) {
	(void) Py_InitModule("pmd_net_system", pmd_net_system_methods);
}
