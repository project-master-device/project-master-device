#include "dev/reader.h"
#include "lib/malloc.h"
#include "lib/interrupt.h"
#include "sys/process.h"
#include "lib/ftimer.h"
#include "contiki-conf.h"

PROCESS(process_handle_reader, "Process for handle reader input");
process_event_t reader_input;

#define WB WIEGAND_BITS-1 // for array index

typedef struct reader_type {
    config_section_t * param;
    uint8_t msg[WIEGAND_BITS];
    uint8_t it;
} reader_t;


void reader_clean_msg(void * data) {
    reader_t * reader = (reader_t *)data;
    //<debug>
    ledd_toggle(5);
    //</debug>
    reader->it = WB;
}

void reader_handle_data0(void * data) {
    reader_t * reader = (reader_t *)data;

    reader->msg[reader->it] = 0;

    if (reader->it == WB) ftimer_register_func(reader_clean_msg, data, 2);

    if (reader->it == 0) {
        process_post(&process_handle_reader, reader_input, data);
        reader->it = WB;
    } else {
        reader->it--;
    }
    interrupt_register(config_section_get_uint(reader->param, "data0_offset", 8), INTERRUPT_FALL, reader_handle_data0, data);
}

void reader_handle_data1(void * data) {
    reader_t * reader = (reader_t *)data;

    reader->msg[reader->it] = 1;

    if (reader->it == WB) ftimer_register_func(reader_clean_msg, data, 2);

    if (reader->it == 0) {
        process_post(&process_handle_reader, reader_input, data);
        reader->it = WB;
    } else {
        reader->it--;
    }
    interrupt_register(config_section_get_uint(reader->param, "data1_offset", 8), INTERRUPT_FALL, reader_handle_data1, data);
}


void reader_init(reader_t * reader) {
    volatile uint8_t * data0_ddr = (volatile uint8_t *)config_section_get_uint(reader->param, "data0_ddr", 0);
    uint8_t data0_offset = config_section_get_uint(reader->param, "data0_offset", 0);

    volatile uint8_t * data1_ddr = (volatile uint8_t *)config_section_get_uint(reader->param, "data1_ddr", 0);
    uint8_t data1_offset = config_section_get_uint(reader->param, "data1_offset", 0);

    *data0_ddr &= ~(1 << data0_offset);
    *data1_ddr &= ~(1 << data1_offset);
}

void reader_create(config_section_t * sect) {
    reader_t * reader = (reader_t *)malloc(sizeof(reader_t));

    reader->param = sect;
    reader_init(reader);

    reader->it = WB;
    process_start(&process_handle_reader, NULL);

    //enable interrupts
    interrupt_register(config_section_get_uint(reader->param, "data0_offset", 8), INTERRUPT_FALL, reader_handle_data0, (void *)reader);
    interrupt_register(config_section_get_uint(reader->param, "data1_offset", 8), INTERRUPT_FALL, reader_handle_data1, (void *)reader);
}

PROCESS_THREAD(process_handle_reader, ev, data) {
    reader_t * reader;
    int i;

    PROCESS_BEGIN();
    reader_input = process_alloc_event();

    for(;;) {
        PROCESS_WAIT_EVENT();
        if (ev == reader_input) {
            reader = (reader_t *) data;
//<debug>
            uint8_t res = 1;

            if (reader->msg[25] == 1) {
                res = res && 1;
            } else {
                res = 0;
            }

            for (i = 24; i >= 0; i--) {
                if (reader->msg[i] == 0) res = res && 1;
                else res = 0;
            }

            if (res == 1) ledd_toggle(4);
//</debug>
            //TODO send msg to master
        }
    }

    PROCESS_END();
}
