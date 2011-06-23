/*
 * Copyright (c) 2011 PMDteam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is a part of PMD Subsystem.
 */

/**
 * @file device.h
 * @brief A library for abstract device.
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include "lib/list.h"
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Types in enum in contiki-conf.h.
 */
typedef int dev_type;

/**
 * @struct device_t
 * @brief A structure for abstract device.
 */
typedef struct {
    void * next; ///< needed for list.

    void * device; ///< the device, converted to void *.
    dev_type type; ///< type of the device.
    uint16_t id; ///< device id.
} device_t;


/**
 * @brief Device list.
 * Stores devices.
 */
LIST(devices);

/**
 * @brief Create a new abstract device.
 *
 * @param dev void * to device.
 * @param type device type (in enum in contiki-conf.h)
 * @return 1 if successful and 0 if not.
 */
unsigned device_create(void * dev, dev_type type);

/**
 * @brief Get device by id.
 *
 * @param dev_id the device id.
 * @return a pointer to the device.
 */
device_t * device_get(uint16_t dev_id);


/**
 * @brief Initialize all devices.
 *
 * @return 1 if successful and 0 if not.
 */
unsigned devices_init();

#endif /* DEVICE_H_ */
