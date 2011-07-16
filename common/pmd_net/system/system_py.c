// created 06.07.2011

#include "system_py.h"

/* ----------------------------------------	HEARTBEAT--------------------------------------------*/

static PyObject* pmd_net_sys_heartbeat_w_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	int rc = pmd_net_sys_heartbeat_write_data(&arr);
	return pmd_net_return_arr(rc, &arr);
}

/* ----------------------------------------CONFIG-----------------------------------------------*/

// ["config_cnf", ("config_section", id -int, ["options", ("option", key -str, value -str/int?, type -str/int?), ...]), ...]
// type: [0;255]
int convert_section_from_py(PyObject *section_py, config_section_t* section) {
    if((section_py == NULL) || (section == NULL))
        return 1;

	config_section_t section_tmp;
	PyObject* options_list_py;
	config_option_t option_tmp;
	PyObject* option_py;
	PyObject* value_py;
	char* value_char;
	config_int_t value_int;
	config_uint_t value_uint;
	int rc = 0;
	char * buf = NULL;
	char * key_buf = NULL;

	// parse config_section_py: python tuple (str - "typename", int - id, python list - options):
	if( !PyArg_ParseTuple(section_py, "sIO", &buf, &(section_tmp.id), &options_list_py) || !PyList_Check(options_list_py) ) {
		PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section expected (str,uint,PyObject)");
		return 1;
	}
	section->id = section_tmp.id;

	// parse options_list_py: python list of tuples - options:
//	type_name = PyList_GetItem(options_list_py, 0); == "options"
	int i;
	for (i = 1; i < PyList_GET_SIZE(options_list_py); i++) {
		option_py = PyList_GetItem(options_list_py, i);
		// parse option_py: python tuple (str - "typename", str - key, ? - value, int - type):q
		if(!PyArg_ParseTuple(option_py, "ssOI", &buf, &key_buf, &value_py, &(option_tmp.type))) {
		    printf("convert_section_from_py fail #2\n");
			PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section:option expected (str,str,PyObject,uint)");
			rc = -1; //TOBEFIXED
		} else {
		    if((strlen(key_buf) + 1) > CONFIG_OPTION_KEY_LEN) {
		        PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section:option key len too long");
		        return 1;
		    }
		    memcpy(option_tmp.key, key_buf, strlen(key_buf) + 1);

			switch (option_tmp.type) {
				case CONFIG_OPTION_STR_TYPE:
					if(!PyString_Check(value_py)) {
						PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section:option:value expected str");
						rc = -1; //TOBEFIXED
					} else {
					    value_char = PyString_AsString(value_py);
						rc = config_section_set_str(section, option_tmp.key, value_char);
					}
					break;
				case CONFIG_OPTION_NUM_INT_TYPE:
				    if(!PyInt_Check(value_py)) {
						PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section:option:value expected int");
						rc = -1; //TOBEFIXED
					} else {
					    value_int = (config_int_t)PyInt_AsLong(value_py);
						rc = config_section_set_int(section, option_tmp.key, value_int);
					}
					break;
				case CONFIG_OPTION_NUM_UINT_TYPE:
                    if(!PyInt_Check(value_py)) {
                        PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf:section:option:value expected uint");
                        rc = -1; //TOBEFIXED
                    } else {
                        value_int = (config_uint_t)PyInt_AsLong(value_py);
                        rc = config_section_set_uint(section, option_tmp.key, value_int);
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
		PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write:cnf expected list, got %s", cnf_py->ob_type->tp_name);
		return 1;
	}

	int i;
	PyObject* section_py;
	config_section_t* section = NULL;
	int rc;
//	PyObject* type_name;
//	type_name = PyList_GetItem(config_cnf_py, 0); == "config_cnf"
	for (i = 1; i < PyList_GET_SIZE(cnf_py); i++) {
		section_py = PyList_GetItem(cnf_py, i);

		section = config_cnf_create_section(cnf, 0);
		rc = convert_section_from_py(section_py, section);

		if (rc) {
		    return 1;
		}
	}
	return 0;
}

inline static PyObject* call_config_write_py(pmd_net_sys_config_data_t* data) {
	bytearr_t arr;
	int rc = pmd_net_sys_config_write_data(&arr, data);
	return pmd_net_return_arr(rc, &arr);
}

static PyObject* pmd_net_sys_config_w_request_py(PyObject* self, PyObject* args) {
	pmd_net_sys_config_data_t data;
	data.operation = PMD_NET_SYS_CONFIG_REQUEST;
	return call_config_write_py(&data);
}

static PyObject* pmd_net_sys_config_w_full_py(PyObject* self, PyObject* args) {
    pmd_net_sys_config_data_t data;
    PyObject* cnf_py = NULL;
    PyObject* res = NULL;

    if(!PyArg_ParseTuple(args, "O", &cnf_py)) {
        PyErr_Format(PyExc_TypeError, "pmd_net_system.pmd_net_system.full -- wtf? it's impossible");
        return Py_BuildValue("(is)", -1, NULL);
    }

    data.section = NULL;
    data.operation = PMD_NET_SYS_CONFIG_FULL;
    data.config = (config_cnf_t *)malloc(sizeof(config_cnf_t));
    if(data.config == NULL) {
        return Py_BuildValue("(is)", -1, NULL);
    }
    config_cnf_construct(data.config);

    int rc = convert_cnf_from_py(cnf_py, data.config);
    if (rc) {
        if(data.config != NULL) {
            config_cnf_destruct(data.config);
            free(data.config);
        }

        return Py_BuildValue("(is)", rc, NULL);
    }

    res = call_config_write_py(&data);

    if(data.config != NULL) {
        config_cnf_destruct(data.config);
        free(data.config);
    }

    return res;
}

inline static PyObject* call_config_write_with_section(PyObject* arg, uint8_t operation) {
	pmd_net_sys_config_data_t data;
	data.operation = operation;
	data.config = NULL;
	PyObject* section_py;
	PyObject* res;

	if(!PyArg_ParseTuple(arg, "O", &section_py)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_system.config_write_section -- wtf? it's impossible");
		return Py_BuildValue("(is)", -1, NULL);
	}

	data.section = (config_section_t *)malloc(sizeof(config_section_t));
	config_section_construct(data.section, 0);

	if ( convert_section_from_py(section_py, data.section) ) {
	    if(data.section != NULL) {
	        config_section_destruct(data.section);
	        free(data.section);
	    }

	    return Py_BuildValue("(is)", -1, NULL);
	}

	res = call_config_write_py(&data);

	if(data.section != NULL) {
	    config_section_destruct(data.section);
	    free(data.section);
	}

	return res;
}

static PyObject* pmd_net_sys_config_w_section_add_py(PyObject* self, PyObject* args) {
	return call_config_write_with_section(args, PMD_NET_SYS_CONFIG_SECTION_ADD);
}

static PyObject* pmd_net_sys_config_w_section_del_py(PyObject* self, PyObject* args) {
	return call_config_write_with_section(args, PMD_NET_SYS_CONFIG_SECTION_DEL);
}

static PyObject* pmd_net_sys_config_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_sys_config_data_t* data = NULL;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_system.config_read - expected string");
		return Py_BuildValue("(is)", -1, NULL);
	}
	int rc = pmd_net_sys_config_read_data(&arr, data);
	return pmd_net_return_op(rc, data->operation);
}

/* ----------------------------------------SET_OP-----------------------------------------------*/

inline static PyObject* call_setop_write(uint8_t operation) {
	bytearr_t arr;
	pmd_net_sys_set_op_data_t data;
	data.operation = operation;
	int rc = pmd_net_sys_set_op_write_data(&arr, &data);
	return pmd_net_return_arr(rc, &arr);
}
static PyObject* pmd_net_setop_w_init_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYS_SET_OP_INIT);
}
static PyObject* pmd_net_setop_w_normal_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYS_SET_OP_NORMAL);
}
static PyObject* pmd_net_setop_w_configuration_py(PyObject* self, PyObject* args) {
	return call_setop_write(PMD_NET_SYS_SET_OP_CONFIGURATION);
}

/*
static PyObject* pmd_net_sys_setop_w_py(PyObject* self, PyObject* args) {
	pmd_net_sys_set_op_data_t data;
	if(!PyArg_ParseTuple(args, "I", &data.operation)) {
		return Py_BuildValue("i", -1);
	}
	return call_setop_write(data.operation);
}
*/

static PyObject* pmd_net_sys_setop_r_py(PyObject *self, PyObject *args) {
	bytearr_t arr;
	pmd_net_sys_set_op_data_t data;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_system.setop_read - expected string");
		return Py_BuildValue("(is)", -1, NULL);
	}
	int rc = pmd_net_sys_set_op_read_data(&arr, &data);
	return pmd_net_return_op(rc, data.operation);
}

/* ----------------------------------------PY_MODULE--------------------------------------------*/

static PyMethodDef pmd_net_system_methods[] = {
	{"heartbeat_w", pmd_net_sys_heartbeat_w_py, METH_VARARGS, "(i)make data for heartbeat msg, return:(rc -int, packed_msg -str)"},
	{"config_w_request", pmd_net_sys_config_w_request_py, METH_VARARGS, "(o?)pack command for config: request full config| args: - | return:(rc -int, packed_msg -str)"},
	{"config_w_full", pmd_net_sys_config_w_full_py, METH_VARARGS, "(i?)pack command for config: write full config| args: config -list| return:(rc -int, packed_msg -str)"},
	{"config_w_section_add", pmd_net_sys_config_w_section_add_py, METH_VARARGS, "(o)pack command for config: add config section| args: section -tuple| return:(rc -int, packed_msg -str)"},
	{"config_w_section_del", pmd_net_sys_config_w_section_del_py, METH_VARARGS, "(o)pack command for config: del config section| args: section -tuple| return:(rc -int, packed_msg -str)"},
//	{"config_write", pmd_net_sys_config_w_py, METH_VARARGS, "pack command for config, return:(rc -int, packed_msg -str)"},
	{"config_read", pmd_net_sys_config_r_py, METH_VARARGS, "unpack command for config| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{"setop_w_init", pmd_net_setop_w_init_py, METH_VARARGS, "pack command: set mode <ititialization> | args: - | return:(rc -int, packed_msg -str)"},
	{"setop_w_normal", pmd_net_setop_w_normal_py, METH_VARARGS, "pack command: set mode <normal> | args: - | return:(rc -int, packed_msg -str)"},
	{"setop_w_configuration", pmd_net_setop_w_configuration_py, METH_VARARGS, "pack command: set mode <configuration> | args: - | return:(rc -int, packed_msg -str)"},
//	{"setop_write", pmd_net_sys_setop_w_py, METH_VARARGS, "pack command for set_op, return:(rc -int, packed_msg -str)"},
	{"setop_read", pmd_net_sys_setop_r_py, METH_VARARGS, "unpack command for set_op| args: packed_msg -str| return:(rc -int, operation_code -int)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_system(void) {
	PyObject * m = Py_InitModule("pmd_net_system", pmd_net_system_methods);

	PyObject * config_dict;
	PyObject * config_dict_value;

	PyObject * option_type_dict;
	PyObject * option_type_dict_value;

	config_dict = PyDict_New();
	Py_INCREF(config_dict);
	config_dict_value = PyInt_FromLong(PMD_NET_SYS_CONFIG_FULL);
	PyDict_SetItemString(config_dict, "FULL", config_dict_value);
	config_dict_value = PyInt_FromLong(PMD_NET_SYS_CONFIG_REQUEST);
	PyDict_SetItemString(config_dict, "REQUEST", config_dict_value);
	config_dict_value = PyInt_FromLong(PMD_NET_SYS_CONFIG_SECTION_ADD);
	PyDict_SetItemString(config_dict, "SECTION_ADD", config_dict_value);
	config_dict_value = PyInt_FromLong(PMD_NET_SYS_CONFIG_SECTION_DEL);
	PyDict_SetItemString(config_dict, "SECTION_DEL", config_dict_value);

	option_type_dict = PyDict_New();
	Py_INCREF(option_type_dict);
	option_type_dict_value = PyInt_FromLong(CONFIG_OPTION_STR_TYPE);
	PyDict_SetItemString(option_type_dict, "STRING", option_type_dict_value);
	option_type_dict_value = PyInt_FromLong(CONFIG_OPTION_NUM_INT_TYPE);
	PyDict_SetItemString(option_type_dict, "INT", option_type_dict_value);
	option_type_dict_value = PyInt_FromLong(CONFIG_OPTION_NUM_UINT_TYPE);
	PyDict_SetItemString(option_type_dict, "UINT", option_type_dict_value);

	PyModule_AddObject(m, "ConfigOperations", config_dict);
	PyModule_AddObject(m, "ConfigOptionTypes", option_type_dict);
}
