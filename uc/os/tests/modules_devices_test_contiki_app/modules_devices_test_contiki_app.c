#include "contiki.h"
#include "lib/config.h"
#include "lib/module.h"
#include "lib/device.h"
#include "lib/module_manager.h"
#include "lib/devices_admin.h"
#include "contiki-conf.h"
#include "list.h"

#include "dev/led.h"
#include "module/led_module/led_module.h"
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>


PROCESS(modules_devices_test_process, "Test for modules and devices");

AUTOSTART_PROCESSES(&modules_devices_test_process);


PROCESS_THREAD(modules_devices_test_process, ev, data) {
    PROCESS_BEGIN();

    device_t * led_dev = admin_get_device_by_id(1);
    device_t * led_m_dev = admin_get_device_by_id(2);

    led_t * led, * led_m;
    if (led_dev && led_m_dev) {
        led = (led_t *)led_dev->device;
        led_m = (led_t *)led_m_dev->device;

        led->init(led);
        led_m->init(led_m);

        while (1) {
            led->off(led);
            led_m->on(led_m);
            _delay_ms(500);
            led->on(led);
            led_m->off(led_m);
            _delay_ms(500);
        }
    }

    PROCESS_END();
}
