#include "lib/device.h"
#include "lib/malloc.h"
#include "lib/module.h"
#include "lib/devices_admin.h"
#include "contiki-conf.h"

#include "dev/led.h"
#include "module/led_module/led_module.h"

unsigned device_create(void * device, dev_type type) {
    device_t * dev = (device_t *) malloc(sizeof(device_t));
    if (dev == NULL)
        return 0;

    dev->device = device;
    dev->type = type;
    // register device in network
    if (interlayer_register_device(dev))
        return 1;
    return 0;
}

unsigned devices_init() {
    led_t * led_on_board = (led_t *) malloc(sizeof(led_t));
    led_t * led_module = (led_t *) malloc(sizeof(led_t));
    int a[1] = {LED1PIN};
    led_on_board->param = (void*) a;
    led_on_board->on = led_on;

    led_module->param = module_get(MODULE_LED);
    led_module->on = led_module_on;

    return device_create((void *) led_on_board, DEVICE_LED) &&
           device_create((void *) led_module, DEVICE_LED);
}
