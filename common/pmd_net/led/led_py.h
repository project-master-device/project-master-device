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


#ifndef PMD_NET_LED_PY_H
#define PMD_NET_LED_PY_H

#include "../pmd_net_py_common.h"
#include "led.h"

//return:(rc -int, packed_msg -string)
static PyObject* pmd_net_led_w_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_led_w_off_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_led_w_toggle_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_led_w_py(PyObject* self, PyObject* args);


//return:(rc -int, operation_code -int)
static PyObject* pmd_net_led_r_py(PyObject* self, PyObject* args);

#endif // PMD_NET_LED_PY_H
