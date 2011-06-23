#include "interrupt.h"
#include <stddef.h>
#include "lib/malloc.h"
#include <avr/interrupt.h>



typedef struct {
    void * data;
    void (*callback)(void *);
} callback_t;

/**
 * @brief An array to manage all 8 interrupts.
 */
static callback_t * irq_handlers[8];


static void interrupt_on(uint8_t num, interrupt_type_t type) {
    volatile uint8_t * cr;

    cli();
    EIMSK |= (1 << num);

    if(num < 4) {
        cr = &EICRA;
    } else if(num < 8) {
        cr = &EICRB;
        num -= 4;
    }

    switch(type) {
        case INTERRUPT_LOWLEVEL:
            *cr &= ~(1 << 2 * num);
            *cr &= ~(1 << (2 * num + 1));
            break;

        case INTERRUPT_ANY:
            *cr |=  (1 << 2 * num);
            *cr &= ~(1 << (2 * num + 1));
            break;

        case INTERRUPT_FALL:
            *cr &= ~(1 << 2 * num);
            *cr |=  (1 << (2 * num + 1));
            break;

        case INTERRUPT_RISE:
            *cr |=  (1 << 2 * num);
            *cr |=  (1 << (2 * num + 1));
            break;
    }

    sei();
}

static void interrupt_off(uint8_t num) {
    EIMSK &= ~(1 << num);
}

void interrupt_init() {
    int j;
    for (j = 0; j < 8; ++j) {
        interrupt_off(j);
        irq_handlers[j] = NULL;
    }
}

//TODO зачем возвращать true-false
uint8_t interrupt_register(uint8_t i, interrupt_type_t type, void (*callb)(void *), void * data) {
    if (irq_handlers[i] != NULL) return 0;
    callback_t * cb = (callback_t *)malloc(sizeof(callback_t));

    cb->callback = callb;
    cb->data = data;

    irq_handlers[i] = cb;
    interrupt_on(i, type);

    return 1;
}

ISR( INT0_vect) {
    callback_t * cb;

    if (irq_handlers[0]) {
        cb = irq_handlers[0];

        interrupt_off(0);
        irq_handlers[0] = NULL;

        cb->callback(cb->data); // PIND, PD0
        free(cb);
    }
}

ISR( INT1_vect) {
    callback_t * cb;

    if (irq_handlers[1]) {
        cb = irq_handlers[1];

        interrupt_off(1);
        irq_handlers[1] = NULL;

        cb->callback(cb->data); // PIND, PD1
        free(cb);
    }
}

ISR( INT2_vect) {
    callback_t * cb;

    if (irq_handlers[2]) {
        cb = irq_handlers[2];

        interrupt_off(2);
        irq_handlers[2] = NULL;

        cb->callback(cb->data); // PIND, PD2
        free(cb);
    }
}

ISR( INT3_vect) {
    callback_t * cb;

    if (irq_handlers[3]) {
        cb = irq_handlers[3];

        interrupt_off(3);
        irq_handlers[3] = NULL;

        cb->callback(cb->data); // PIND, PD3
        free(cb);
    }
}

ISR( INT4_vect) {
    callback_t * cb;

    if (irq_handlers[4]) {
        cb = irq_handlers[4];

        interrupt_off(4);
        irq_handlers[4] = NULL;

        cb->callback(cb->data); // PINE, PE4
        free(cb);
    }
}

ISR( INT5_vect) {
    callback_t * cb;
    if (irq_handlers[5]) {
        cb = irq_handlers[5];

        interrupt_off(5);
        irq_handlers[5] = NULL;

        cb->callback(cb->data); // PINE, PE5
        free(cb);
    }
}

ISR( INT6_vect) {
    callback_t * cb;
    if (irq_handlers[6]) {
        cb = irq_handlers[6];

        interrupt_off(6);
        irq_handlers[6] = NULL;

        cb->callback(cb->data); // PINE, PE6
        free(cb);
    }
}

ISR( INT7_vect) {
    callback_t * cb;

    if (irq_handlers[7]) {
        cb = irq_handlers[7];

        interrupt_off(7);
        irq_handlers[7] = NULL;

        cb->callback(cb->data); // PINE, PE7
        free(cb);
    }
}
