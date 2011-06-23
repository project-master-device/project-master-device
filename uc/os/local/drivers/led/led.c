#include "dev/led.h"
#include "lib/config.h"
#include "lib/malloc.h"
#include <stdint.h>
#include <stddef.h>


/**
 * @struct led_t
 * It contains pointers to functions which must be realized in specific platform.
 * They must be set in initializations of devices.
 */
typedef struct led_type {
    config_section_t * param;
} led_t;


void led_init(led_t * led_m) {
    volatile uint8_t * ddr = config_section_get_uint(led_m->param, "ddr", 0);
    uint8_t offset = config_section_get_uint(led_m->param, "offset", 0);

    *ddr |= (1 << offset);
}


void led_on(led_t * led_m) {
    volatile uint8_t * port = config_section_get_uint(led_m->param, "port", 0);
    uint8_t offset = config_section_get_uint(led_m->param, "offset", 0);

    *port |= (1 << offset);
}
void led_off(led_t * led_m) {
    volatile uint8_t * port = config_section_get_uint(led_m->param, "port", 0);
    uint8_t offset = config_section_get_uint(led_m->param, "offset", 0);

    *port &= ~(1 << offset);
}
void led_toggle(led_t * led_m) {
    volatile uint8_t * port = config_section_get_uint(led_m->param, "port", 0);
    uint8_t offset = config_section_get_uint(led_m->param, "offset", 0);

    *port ^= (1 << offset);
}

void led_create(config_section_t * sect) {
    led_t * led = (led_t *)malloc(sizeof(led_t));

    led->param = sect;

    led_init(led);
}


