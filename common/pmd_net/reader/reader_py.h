// created 06.07.2011

#ifndef PMD_NET_LED_PY_H
#define PMD_NET_LED_PY_H

#include <Python.h>
#include "reader.h"

#define PMD_NET_READER_SEND_MSG         0
#define PMD_NET_READER_GREEN_LED_ON     1
#define PMD_NET_READER_GREEN_LED_OFF    2
#define PMD_NET_READER_BEEP_ON          3
#define PMD_NET_READER_BEEP_OFF         4

//return:(rc -int, packed_msg -string)
static PyObject* pmd_net_reader_write_send_msg_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_write_green_led_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_write_green_led_off_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_write_beep_on_py(PyObject* self, PyObject* args);
static PyObject* pmd_net_reader_write_beep_off_py(PyObject* self, PyObject* args);
//static PyObject* pmd_net_reader_write_data_py(PyObject* self, PyObject* args);


//return:(rc -int, operation_code -int)
static PyObject* pmd_net_reader_read_data_py(PyObject* self, PyObject* args);

#endif // PMD_NET_LED_PY_H
