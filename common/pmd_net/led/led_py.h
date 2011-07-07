// created 06.07.2011

#ifndef PMD_NET_LED_PY_H
#define PMD_NET_LED_PY_H

#include <Python.h>
#include "led.h"

//return:(rc -int, packed_msg -string)
static PyObject* pmd_net_led_write_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_led_write_off_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_led_write_toggle_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_led_write_data_py(PyObject* self, PyObject* args);


//return:(rc -int, operation_code -int)
static PyObject* pmd_net_led_read_data_py(PyObject* self, PyObject* args);

#endif // PMD_NET_LED_PY_H
