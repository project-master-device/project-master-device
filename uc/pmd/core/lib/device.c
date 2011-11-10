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


#include "device.h"
#include "malloc.h"
#include "config.h"
#include "contiki-conf.h"

#include "dev/led.h"
#include "dev/button.h"
#include "dev/reader.h"
#include <string.h>


unsigned device_create(void * device, dev_type type) {
    device_t * dev = (device_t *) malloc(sizeof(device_t));
    if (dev == NULL)
        return 0;

    dev->device = device;
    dev->type = type;

    //give id to the device
    uint16_t id = 1;
    if (devices_list != 0) {
        id = ((device_t *) list_tail(devices))->id + 1;
    }
    dev->id = id;
    list_add(devices, (void *) dev);
    return 1;
}

device_t * device_get(uint16_t dev_id) {
    device_t * dev;
    for (dev = (device_t *) list_head(devices); dev != 0;
            dev = (device_t *) list_item_next((void *) dev)) {
        if (dev->id == dev_id) return dev;
    }
    return NULL;
}


