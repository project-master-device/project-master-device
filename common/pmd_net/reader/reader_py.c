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


#include "reader_py.h"

//TODO: do it with macros for all simple lvl3 protocols: 'PMD_NET_WRITE_FUNK(reader, send_msg, READER_SEND_MSG)'
inline static PyObject* call_reader_write(uint8_t operation, unsigned int code) {
	bytearr_t arr;
	pmd_net_reader_data_t data;
	data.operation = operation;
	if (code)
		memcpy(&data.data, (uint8_t*)&code, PMD_NET_READER_MSG_LEN);
	int rc = pmd_net_reader_write_data(&arr, &data);
	return pmd_net_return_arr(rc, &arr);
}

static PyObject* pmd_net_reader_w_send_msg_py(PyObject* self, PyObject* args) {
	unsigned int code;
	if(!PyArg_ParseTuple(args, "I", &code)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_reader.w_send_msg expected uint");
		return Py_BuildValue("(is)", -1, NULL);
	}
	return call_reader_write(PMD_NET_READER_SEND_MSG, code);
}
static PyObject* pmd_net_reader_w_green_led_on_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_GREEN_LED_ON, 0);
}
static PyObject* pmd_net_reader_w_green_led_off_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_GREEN_LED_OFF, 0);
}
static PyObject* pmd_net_reader_w_beep_on_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_BEEP_ON, 0);
}
static PyObject* pmd_net_reader_w_beep_off_py(PyObject* self, PyObject* args) {
	return call_reader_write(PMD_NET_READER_BEEP_OFF, 0);
}

/*
static PyObject* pmd_net_reader_w_py(PyObject* self, PyObject* args) {
	pmd_net_reader_data_t data;
	if(!PyArg_ParseTuple(args, "I", &data.operation)) {
		return Py_BuildValue("(is)", rc, NULL);
	}
	return call_reader_write(data.operation);
}
*/
static PyObject* pmd_net_reader_r_py(PyObject* self, PyObject* args) {
	bytearr_t arr;
	pmd_net_reader_data_t data;
	if(!PyArg_ParseTuple(args, "s#", &arr.itself, &arr.len)) {
		PyErr_Format(PyExc_TypeError, "pmd_net_reader.read expected string");
		return Py_BuildValue("(iss)", -1, NULL, NULL);
	}
	int rc = pmd_net_reader_read_data(&arr, &data);
	if (rc) {
		return Py_BuildValue("(iss)", rc, NULL, NULL);
	} else {
		if (data.operation == PMD_NET_READER_SEND_MSG)
			return Py_BuildValue("(iIs#)", rc, data.operation, &data.data, PMD_NET_READER_MSG_LEN);
		else
			return Py_BuildValue("(iIs)", rc, data.operation, NULL);
	}
}

static PyMethodDef pmd_net_reader_methods[] = {
	{"w_send_msg", pmd_net_reader_w_send_msg_py, METH_VARARGS, "(i)pack command: the read code| args: code -uint| return:(rc -int, packed_msg -str)"},
	{"w_green_led_off", pmd_net_reader_w_green_led_off_py, METH_VARARGS, "(o)pack command: turn green led off| args: - | return:(rc -int, packed_msg -str)"},
	{"w_green_led_on", pmd_net_reader_w_green_led_on_py, METH_VARARGS, "(o)pack command: turn green led on| args: - | return:(rc -int, packed_msg -str)"},
	{"w_beep_on", pmd_net_reader_w_beep_on_py, METH_VARARGS, "(o)pack command: turn beeper on| args: - | return:(rc -int, packed_msg -str)"},
	{"w_beep_off", pmd_net_reader_w_beep_off_py, METH_VARARGS, "(o)pack command: turn beeper off| args: - | return:(rc -int, packed_msg -str)"},
//	{"write", pmd_net_reader_w_py, METH_VARARGS, "pack command for reader, return:(rc -int, packed_msg -str)"},
	{"read", pmd_net_reader_r_py, METH_VARARGS, "unpack command for reader| args: packed_msg -str| return:(rc -int, operation_code -int, code -str)"},
	{NULL, NULL, 0, NULL}	/* Sentinel */
};

PyMODINIT_FUNC initpmd_net_reader(void) {
	(void) Py_InitModule("pmd_net_reader", pmd_net_reader_methods);
}
