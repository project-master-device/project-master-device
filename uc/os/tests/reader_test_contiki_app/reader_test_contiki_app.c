#include "contiki.h"
#include "lib/malloc.h"
#include "lib/config.h"
#include "net/can_net.h"
#include "lib/ftimer.h"
#include "lowlevel.h"
#include <util/delay.h>
#include <avr/io.h>

#include "dev/reader.h"
void reader_green_led_off(config_section_t * sect);
void reader_green_led_on(config_section_t * sect);
void reader_beep_off(config_section_t * sect);
void reader_beep_on(config_section_t * sect);

config_section_t * search_sect(config_cnf_t * conf, uint8_t id) {
    config_section_t * sec;

    if(conf == NULL)
        return NULL;

    for (sec = (config_section_t *) list_head(conf->sections); sec != NULL; sec = (config_section_t *) list_item_next(
            (void *) sec)) {
        if (sec->id == id)
            return sec;
    }
}

void reader_led_init() {
    DDRA |= (1 << PA7);
}
void reader_led_on() {
    PORTA &= ~(1 << PA7);
}
void reader_led_off() {
    PORTA |= (1 << PA7);
}
void reader_led_blink(uint8_t n) {
    int i;
    for (i = 0; i < n; i++) {
        reader_led_on();
        _delay_ms(300);
        reader_led_off();
        _delay_ms(300);
    }
}

PROCESS(test_process, "");

AUTOSTART_PROCESSES(&test_process);

int green_led_state = 0;
config_section_t * reader_s = NULL;
void timer_func(void * p) {
    if (green_led_state == 0) {
        green_led_state = 1;
        reader_green_led_on(reader_s);
    } else if (green_led_state == 1) {
        green_led_state = 0;
        reader_green_led_off(reader_s);
    }
    ftimer_register_func(timer_func, NULL, 100);
}

PROCESS_THREAD(test_process, ev, data) {

    PROCESS_BEGIN();
    reader_s = search_sect(config_get(), 5);
    if(reader_s == NULL)
        led3_blink(5, 400);
    else
        ftimer_register_func(timer_func, NULL, 100);

    PROCESS_END();
}
