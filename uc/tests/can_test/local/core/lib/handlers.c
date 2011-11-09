#include "handlers.h"

void handle_led(void * dev) {
    led_t * led = (led_t *)dev;
    //if msg == 01
    led->on(led);
}
