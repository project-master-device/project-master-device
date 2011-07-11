// created 06.07.2011

#ifndef PMD_NET_BUTTON_PY_H
#define PMD_NET_BUTTON_PY_H

#include "../pmd_net_py_common.h"
#include "button.h"

static PyObject* pmd_net_button_w_down_py(PyObject* self, PyObject* args);

static PyObject* pmd_net_button_w_up_py(PyObject* self, PyObject* args);

//static PyObject* pmd_net_button_w_py(PyObject* self, PyObject* args);

static PyObject* pmd_net_button_r_py(PyObject* self, PyObject* args);

//TODO:
//static PyObject* pmd_net_button_op_down_py(PyObject* self, PyObject* args) {
//		return return Py_BuildValue("i", PMD_NET_BUTTON_DOWN);
//}

#endif // PMD_NET_BUTTON_PY_H
