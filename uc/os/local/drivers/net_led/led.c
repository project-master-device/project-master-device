#include "dev/led.h"
#include "lib/config.h"
#include "lib/malloc.h"
#include "pmd_net/pmd_led.h"

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
    int rc;

    if(net_msg != NULL) {
        pmd_led_data_t pmd_led_data;
        rc = pmd_led_read_data(net_msg->data, &pmd_led_data);

        if(rc == 0) {
            switch(pmd_led_data.operation) {
                case PMD_NET_LED_ON:
                    led_on(conf_sect);
                    break;

                case PMD_NET_LED_OFF:
                    led_off(conf_sect);
                    break;

                case PMD_NET_LED_TOGGLE:
                    led_toggle(conf_sect);
                    break;
            }
        }
    }
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
