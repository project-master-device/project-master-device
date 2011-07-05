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
