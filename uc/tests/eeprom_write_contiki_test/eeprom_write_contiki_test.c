/*
 * eeprom_write_contiki_test.c
 *
 *  Created on: 28.03.2011
 *      Author: gsmetal
 */

#include "contiki.h"
#include "lib/config.h"
#include "lib/module.h"
#include "lib/device.h"
#include "lib/module_manager.h"
#include "lib/devices_admin.h"
#include "contiki-conf.h"
#include "list.h"
#include "cfs/cfs.h"

#include "dev/led.h"
#include "module/led_module/led_module.h"
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>


PROCESS(eeprom_write_contiki_test, "Test for writing config to eeprom");
PROCESS(cfs_write_test, "Test for cfs to eeprom");
PROCESS(config_write_test, "Test for config to eeprom");

AUTOSTART_PROCESSES(&eeprom_write_contiki_test);

PROCESS_THREAD(cfs_write_test, ev, data) {
    PROCESS_BEGIN();

    char str[20], str2[20];
    strcpy(str, "Vot bl'a nahuj!");

    int fd = cfs_open("a", CFS_WRITE);
    if (fd == -1) {
        cfs_close(fd);
        fd = cfs_open("a", CFS_WRITE);
        if (fd == -1) {
            alarma(5);
        }
    }
    cfs_write(fd, (void *)str, strlen(str));
    cfs_close(fd);

    fd = cfs_open("a", CFS_READ);
    cfs_read(fd, (void *)str2, strlen(str));
    cfs_close(fd);

    strcat(str2, " Da yopt!");

    fd = cfs_open("a", CFS_WRITE);
    cfs_write(fd, (void *)str2, strlen(str2));
    cfs_close(fd);

    ledd_on(PB5);
    PROCESS_END();
}


PROCESS_THREAD(config_write_test, ev, data) {
    PROCESS_BEGIN();

    config_section_t *sect1, *sect2;

    config_init();
    config_open();

    sect1 = config_cnf_create_section(config_get());
    strcpy(sect1->name, "sect");
    config_section_set_int(sect1, "ololoint", -666);
    config_section_set_uint(sect1, "ololoUint", 1313);
    config_section_set_str(sect1, "ololostr", "znachenie");

    config_save();
    config_close();

    config_init();
    config_open();
////    sect2 = config_cnf_create_section(config_get());
////    strcpy(sect2->name, "sect");
////    config_section_set_int(sect2, "alalaint", -999);
////    config_section_set_uint(sect2, "alalaUint", 3131);
////    config_section_set_str(sect2, "alalastr", "justanother");
    config_save();
    config_close();

    ledd_on(PB5);

    PROCESS_END();
}




PROCESS_THREAD(eeprom_write_contiki_test, ev, data) {
    PROCESS_BEGIN();

    static config_section_t * led_conf = 0;
    int i;
    led_conf = config_cnf_create_section(config_get());

    strcpy(led_conf->name, "led");
    config_section_set_uint(led_conf, "ddr", 39);
    config_section_set_uint(led_conf, "port", 40);
    config_section_set_uint(led_conf, "offset", 0);

    config_save();

    config_close();

    config_init();
    if (config_open() == -1) alarma(10);

    modules_init();
    devices_init();

    if (admin_get_device_by_id(2) && admin_get_device_by_id(1)) {
        static led_t * led_m = 0;
        static led_t * led = 0;

        led_m = (led_t *)admin_get_device_by_id(2)->device;
        led = (led_t *)admin_get_device_by_id(1)->device;
        led->init(led);
        led_m->init(led_m);

        if (module_get(MODULE_LED1)->config == led_conf) {
            for (i = 0; i < 10; ++i) {
                 led->on(led);
                 _delay_ms(50);
                 led->off(led);
                 _delay_ms(50);
            }
        }

        led->on(led);
        led_m->on(led_m);
    }
    PROCESS_END();
}



