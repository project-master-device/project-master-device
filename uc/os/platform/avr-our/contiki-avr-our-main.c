#include "contiki.h"
#include "pmd_subsystem.h"
#include "lib/interrupt.h"
#include "contiki-conf.h"
#include "lib/malloc.h"

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>

#include "dev/button.h"

#define DEBUG

#ifdef DEBUG
    void ledd_on(uint8_t pb) {
            PORTB |= (1 << pb);
    }

    void ledd_off(uint8_t pb) {
            PORTB &= ~(1 << pb);
    }

    void ledd_toggle(uint8_t pb) {
            PORTB ^= (1 << pb);
    }
    void alarma() {
        int i;
        for (i = 0; i < 5; ++i) {
             ledd_on(PB5);
             ledd_off(PB6);
             _delay_ms(50);
             ledd_off(PB5);
             ledd_on(PB6);
             _delay_ms(50);
             ledd_off(PB6);
        }
    }
    void alarma_i(int n) {
        int i;
        for (i = 0; i < n; ++i) {
             ledd_on(PB5);
             ledd_off(PB6);
             _delay_ms(50);
             ledd_off(PB5);
             ledd_on(PB6);
             _delay_ms(50);
             ledd_off(PB6);
             _delay_ms(200);
         }
        _delay_ms(1000);
    }


    void led_blink(int a, uint8_t pb, int ms) {
        int i;
        for (i=0; i < a; ++i) {
            PORTB |= (1 << pb);
            _delay_ms(ms);
            PORTB &= ~(1 << pb);
            _delay_ms(ms);
        }
//        _delay_ms(500);
    }

    void blink() {
        led_blink(10, PB4, 70);
    }


    void init_led(int pb) {
        DDRB |= (1 << pb);
        PORTB &= ~(1 << pb);
    }
#endif
//PROCESS_NAME(process_handle_button);
//PROCESS_NAME(simple_handle_button);
//PROCINIT(&process_handle_button, &simple_handle_button);


int main(void) {
    //?
    //clock_init();


    /* Initialize hardware (for debug)*/
    init_led(PB6);
    init_led(PB5);
    init_led(PB4);


    /* Start process handler */
    process_init();

    /* Register initial processes */
//    procinit_init();

    /* Initialize pmd subsystem */
    if ( !pmd_init() )
        alarma();


    _delay_ms(1000);
    /* Start autostart processes (defined in Contiki application) */
    autostart_start(autostart_processes);




    while (1) {
        process_run();
    }
    return 0;
}

