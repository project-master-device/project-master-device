#ifndef LED_H_
#define LED_H_

#include "lib/module.h"

//i don't know... hard useless way?
typedef struct led_type {
    void * param;
    void (*on)(struct led_type *);
    void (*off)(struct led_type *);
    void (*toggle)(struct led_type *);
} led_t;


void led_on(led_t *);
void led_off(led_t *);
void led_toggle(led_t *);

#endif /* LED_H_ */
