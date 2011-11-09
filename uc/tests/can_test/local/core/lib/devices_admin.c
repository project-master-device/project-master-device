#include "devices_admin.h"
#include "malloc.h"


unsigned admin_enable_device(device_t * dev) {
    device_res_t * dr;
    dr = (device_res_t *) malloc(sizeof(device_res_t));
    if (dr == 0)
        return 0;
    dr->device = dev;

    uint16_t id = 1;
    if (drl_list != 0) {
        id = ((device_res_t *) list_tail(drl))->id + 1;
    }
    dr->id = id;
    list_add(drl, (void *) dr);
    return 1;
}

void admin_disable_device_by_id(uint16_t id) {
    device_res_t * dr;
    for (dr = (device_res_t *) list_head(drl); dr != 0;
            dr = (device_res_t *) list_item_next((void *) dr)) {
        if (dr->id == id) list_remove(drl, dr);
    }
}

device_t * admin_get_device_by_id(uint16_t id) {
    device_res_t * dr;
    for (dr = (device_res_t *) list_head(drl); dr != 0;
            dr = (device_res_t *) list_item_next((void *) dr)) {
        if (dr->id == id) return dr->device;
    }
    return 0;
}
