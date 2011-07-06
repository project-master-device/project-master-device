// created 06.07.2011

#ifndef PMD_NET_LED_PY_H
#define PMD_NET_LED_PY_H

#include <Python.h>
#include "led.h"

static PyObject* pmd_net_led_write_data_py(PyObject *self, PyObject *args);

static PyObject* pmd_net_led_read_data_py(PyObject *self, PyObject *args);

#endif // PMD_NET_LED_PY_H
