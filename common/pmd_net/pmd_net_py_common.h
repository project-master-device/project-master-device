// created 08.07.2011

#ifndef PMD_NET_COMMON_H
#define PMD_NET_COMMON_H

#include <Python.h>
#include <stdint.h>
#include "../lib/bytearray.h"

inline static PyObject* pmd_net_return_arr(int rc, bytearr_t* arr) {
	if (rc)
		return Py_BuildValue("(is)", rc, NULL);
	else
		return Py_BuildValue("(is#)", rc, arr->itself, arr->len);
}

inline static PyObject* pmd_net_return_op(int rc, uint8_t op) {
	if (rc)
		return Py_BuildValue("(is)", rc, NULL);
	else
		return Py_BuildValue("(iI)", rc, op);
}

#endif // PMD_NET_COMMON_H
