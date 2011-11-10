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
 * @file net_device.h
 * @brief A library for abstract network device.
 */

#ifndef NET_DEVICE_H_
#define NET_DEVICE_H_

#include "lib/list.h"
#include "lib/config.h"
#include "net/can_net.h"

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Types in enum in contiki-conf.h.
 */
typedef int dev_type;

/**
 * @brief Network callback function type.
 */
struct net_device_t;
typedef void (*net_callback_t)(config_section_t *, const msg_lvl2_t *);

/**
 * @struct device_t
 * @brief A structure for abstract network device.
 */
typedef struct {
    void * next; ///< needed for list.

    config_section_t * conf_sect; ///< configuration section of the device.
    net_callback_t net_callback;
} net_device_t;

/**
 * @brief Device list.
 * Stores network devices.
 */
extern list_t net_device_list;

/**
 * @brief Create a new abstract device.
 *
 * @param dev void * to device.
 * @param type device type (in enum in contiki-conf.h).
 * @return new device id, -1 if creation failed.
 */
void net_device_construct(net_device_t * net_device, config_section_t * conf_sect, net_callback_t net_callback);
void net_device_destruct(net_device_t * net_device);

/**
 * @brief Get device by id.
 *
 * @param dev_id the device id.
 * @return a pointer to the network device.
 */
net_device_t * net_device_get(uint16_t dev_id);

/**
 * @brief Initialize net device subsystem.
 *
 * @return: return codes:
 *      0 -- successful,
 *      1 -- config problem,
 *      2 -- no type option in one of the sections,
 *      3 -- failed to initialize device driver,
 *      4 -- failed to create net_device.
 */
int net_device_init();
int net_device_terminate();

/**
 * @brief Initialize platform-specific network devices.
 * This function is called from net_device_init function and should be defined in each platform.
 * It is strongly recommended to use the following return codes as defined here
 *      and other -- for platform-specific errors.
 *
 * @return: return codes:
 * FIXME: No need in this functions as external functions
 */
//int net_device_platform_init();
//int net_device_platform_terminate();

#endif /* NET_DEVICE_H_ */
