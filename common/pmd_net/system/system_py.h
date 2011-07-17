// created 06.07.2011

#ifndef PMD_NET_SYS_PY_H
#define PMD_NET_SYS_PY_H

#include "../pmd_net_py_common.h"
#include "system_ids.h"
#include "config.h"
#include "heartbeat.h"
#include "set_op.h"


// WRITERS: return:(rc -int, packed_msg -string)
// READERS: return:(rc -int, operation_code -int)

/* ----------------------------------------	HEARTBEAT--------------------------------------------*/

static PyObject* pmd_net_sys_heartbeat_w_py(PyObject* self, PyObject* args);

/* ----------------------------------------CONFIG-----------------------------------------------*/

static PyObject* pmd_net_sys_config_w_request_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_w_full_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_w_section_add_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_w_section_del_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_sys_config_w_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_config_r_py(PyObject* self, PyObject* args);

/* ----------------------------------------SET_OP-----------------------------------------------*/

static PyObject* pmd_net_set_op_w_init_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_set_op_w_normal_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_set_op_w_configuration_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_sys_set_op_w_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_sys_set_op_r_py(PyObject* self, PyObject* args);

#endif // PMD_NET_SYS_PY_H
