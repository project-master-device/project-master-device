#include "led_module.h"
#include "config.h"

void led_module_on(led_t * led) {
    config_uint_t pin;
    module_t * module = (module_t *)led->param;


    pin = config_section_get_uint(module->config, "pin", 0);
    //TODO подать на пин высокий уровень
}
