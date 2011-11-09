#ifndef DEVICES_ADMIN_H_
#define DEVICES_ADMIN_H_

#include "list.h"
#include "device.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    device_t * device;
    uint16_t id;
} device_res_t; //device resolution struct

LIST(drl); //device resolution list

unsigned admin_enable_device(device_t *);
void admin_disable_device_by_id(uint16_t);
device_t * admin_get_device_by_id(uint16_t);

/*
 * Получится, что в основном цикле будет что-то типо
 * device = interlayer_get_device_by_id(dev_id);
 * switch(device->device_type) {
 *     case DEVICE_LED:
 *         handle_led(device->device, msg);
 *     case DEVICE_BUTTON:
 *         handle_button(device-device, msg);
 * }
 */

#endif /* DEVICES_ADMIN_H_ */
