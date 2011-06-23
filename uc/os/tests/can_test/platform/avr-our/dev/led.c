#include "dev/led.h"

void led_on(led_t * led) {
    int pin = *(int *)led->param;

    //TODO подать на пин высокий уровень

}
