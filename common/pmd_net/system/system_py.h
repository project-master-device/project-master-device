// created 06.07.2011

#ifndef PMD_NET_SYS_PY_H
#define PMD_NET_SYS_PY_H

#include <Python.h>
#include "config.h"
#include "heartbeat.h"
#include "set_op.h"

static PyObject* pmd_net_heartbeat_write_data_py(PyObject *self, PyObject *args);

static PyObject* pmd_net_sys_config_write_data_py(PyObject *self, PyObject *args);
static PyObject* pmd_net_sys_config_read_data_py(PyObject *self, PyObject *args);

static PyObject* pmd_net_sys_set_op_write_data_py(PyObject *self, PyObject *args);
static PyObject* pmd_net_sys_set_op_read_data_py(PyObject *self, PyObject *args);

#endif // PMD_NET_SYS_PY_H
