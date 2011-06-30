#include "net_device.h"

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
    config_int_t id;

    for (dev = (net_device_t *)list_head(net_device_list); dev != NULL;
            dev = (net_device_t *)list_item_next((void *)dev)) {

        if(dev->conf_sect->id == dev_id) {
            return dev;
        }
    }
    return NULL;
}

int net_device_init() {
    list_init(net_device_list);
    int rc = net_device_platform_init();

    return rc;
}
