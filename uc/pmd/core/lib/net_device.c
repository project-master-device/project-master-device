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


#include "net_device.h"
#include "dev/led.h"
#include "dev/button.h"
#include "dev/reader.h"

#include <string.h>

static void * net_device_list_begin = NULL;
list_t net_device_list = (list_t)(&net_device_list_begin);


void net_device_construct(net_device_t * net_device, config_section_t * conf_sect, net_callback_t net_callback) {
    if(net_device == NULL)
        return;

    net_device->next = NULL;
    net_device->conf_sect = conf_sect;
    net_device->net_callback = net_callback;

    list_add(net_device_list, (void *)net_device);
}

void net_device_destruct(net_device_t * net_device) {
    if(net_device == NULL)
        return;

    list_remove(net_device_list, (void *)net_device);
}

net_device_t * net_device_get(uint16_t dev_id) {
    net_device_t * dev;

    for (dev = (net_device_t *)list_head(net_device_list); dev != NULL;
            dev = (net_device_t *)list_item_next((void *)dev)) {

        if(dev->conf_sect->id == dev_id) {
            return dev;
        }
    }
    return NULL;
}

int net_device_platform_init() {
    config_section_t * sect = NULL;
    net_device_t * net_device = NULL;
    char type[16];

    config_cnf_t * cnf = config_get();
    if(cnf == NULL) return 1;

    for(sect = (config_section_t *)list_head(cnf->sections); sect != NULL; sect = (config_section_t *)list_item_next((void*)sect)) {
        config_section_get_str(sect, "type", NULL, type, strlen(type));
        if (type == NULL)
            return 2;

        if (strcmp(type, "led") == 0) {
            if(led_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, led_net_callback);

        } else if (strcmp(type, "button") == 0) {
            if(button_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, NULL);

        } else if (strcmp(type, "reader") == 0) {
            if(reader_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, reader_net_callback);
        }
        // etc. new drivers
    }

    return 0;
}

int net_device_init() {
    list_init(net_device_list);
    int rc = net_device_platform_init();

    return rc;
}

int net_device_terminate() {
    net_device_t * it = NULL;
    net_device_t * next = NULL;

    for(it = (net_device_t *)list_head(net_device_list); it != NULL; it = next) {
        next = (net_device_t *)list_item_next((void *)it);
        free(it);
    }
    list_init(net_device_list);

    return 0;
}
