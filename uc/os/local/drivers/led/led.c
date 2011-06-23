#include "dev/led.h"
#include "lib/config.h"
#include "lib/malloc.h"
#include <stdint.h>
#include <stddef.h>


int led_init(config_section_t * conf_sect) {
    volatile uint8_t * ddr = (uint8_t *)config_section_get_uint(conf_sect, "ddr", (config_uint_t)NULL);
    if(ddr == NULL) return 1;

    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);
    *ddr |= (1 << offset);

    return 0;
}

void led_net_callback(config_section_t * conf_sect, msg_lvl2_t * net_msg) {
    //TODO
}


void led_on(config_section_t * conf_sect) {
    volatile uint8_t * port = (uint8_t *)config_section_get_uint(conf_sect, "port", (config_uint_t)NULL);
    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);

    if(port != NULL) {
        *port |= (1 << offset);
    }
}

void led_off(config_section_t * conf_sect) {
    volatile uint8_t * port = (uint8_t *)config_section_get_uint(conf_sect, "port", (config_uint_t)NULL);
    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);

    if(port != NULL) {
        *port &= ~(1 << offset);
    }
}

void led_toggle(config_section_t * conf_sect) {
    volatile uint8_t * port = (uint8_t *)config_section_get_uint(conf_sect, "port", (config_uint_t)NULL);
    uint8_t offset = config_section_get_uint(conf_sect, "offset", 0);

    if(port != NULL) {
        *port ^= (1 << offset);
    }
}
