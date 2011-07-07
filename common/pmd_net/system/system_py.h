// created 06.07.2011

#ifndef PMD_NET_SYS_PY_H
#define PMD_NET_SYS_PY_H

#include <Python.h>
#include "config.h"
#include "heartbeat.h"
#include "set_op.h"

// WRITERS: return:(rc -int, packed_msg -string)
// READERS: return:(rc -int, operation_code -int)

/* ----------------------------------------	HEARTBEAT--------------------------------------------*/

static PyObject* pmd_net_heartbeat_write_py(PyObject* self, PyObject* args);

/* ----------------------------------------CONFIG-----------------------------------------------*/

static PyObject* pmd_net_sys_config_write_config_request_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_write_config_full_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_write_config_section_add_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_write_config_section_del_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_sys_config_write_data_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_read_data_py(PyObject* self, PyObject* args);

/* ----------------------------------------SET_OP-----------------------------------------------*/

//static PyObject* pmd_net_sys_set_op_write_data_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_set_op_read_data_py(PyObject* self, PyObject* args);

#endif // PMD_NET_SYS_PY_H
