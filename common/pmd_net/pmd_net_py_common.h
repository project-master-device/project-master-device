/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


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
