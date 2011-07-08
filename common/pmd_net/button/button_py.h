// created 06.07.2011

#ifndef PMD_NET_BUTTON_PY_H
#define PMD_NET_BUTTON_PY_H

#include <Python.h>
#include "button.h"

static PyObject* pmd_net_button_write_down_py(PyObject* self, PyObject* args);

static PyObject* pmd_net_button_write_up_py(PyObject* self, PyObject* args);

//static PyObject* pmd_net_button_write_data_py(PyObject* self, PyObject* args);

static PyObject* pmd_net_button_read_data_py(PyObject* self, PyObject* args);

#endif // PMD_NET_BUTTON_PY_H
