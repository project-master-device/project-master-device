/*
 * Copyright (c) 2010-2011 Sergey Gnuskov, Timofey Kirillov
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
