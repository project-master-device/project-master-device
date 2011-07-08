// created 06.07.2011

#ifndef PMD_NET_LED_PY_H
#define PMD_NET_LED_PY_H

#include <Python.h>
#include "reader.h"

//return:(rc -int, packed_msg -string)
static PyObject* pmd_net_reader_w_send_msg_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_w_green_led_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_w_green_led_off_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_w_beep_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_w_beep_off_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_reader_w_py(PyObject* self, PyObject* args);


//return:(rc -int, operation_code -int)
static PyObject* pmd_net_reader_r_py(PyObject* self, PyObject* args);

#endif // PMD_NET_LED_PY_H
