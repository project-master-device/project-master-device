#include "dev/reader.h"
#include "lib/malloc.h"
#include "lib/interrupt.h"
#include "sys/process.h"
#include "lib/ftimer.h"
#include "contiki-conf.h"
#include "pmd_net/pmd_reader.h"
#include "net/can_net_middle.h"

PROCESS(process_handle_reader, "Process for handle reader input");
process_event_t reader_input;

#define WB (WIEGAND_BITS-1)// for iterator
typedef struct reader_type {
    config_section_t * param;
    uint64_t msg;
    uint8_t it;
} reader_t;

// periphery control
void reader_green_led_off(config_section_t * sect) {
    volatile uint8_t * green_led_port = (volatile uint8_t *) config_section_get_uint(sect, "gled_port", 0);
    uint8_t green_led_offset = config_section_get_uint(sect, "gled_offset", 100);

    if ((green_led_port != 0) && (green_led_offset != 100))
        *green_led_port |= (1 << green_led_offset);
}

void reader_green_led_on(config_section_t * sect) {
    volatile uint8_t * green_led_port = (volatile uint8_t *) config_section_get_uint(sect, "gled_port", 0);
    uint8_t green_led_offset = config_section_get_uint(sect, "gled_offset", 100);

    if ((green_led_port != 0) && (green_led_offset != 100))
        *green_led_port &= ~(1 << green_led_offset);
}
void reader_beep_off(config_section_t * sect) {
    volatile uint8_t * beep_port = (volatile uint8_t *) config_section_get_uint(sect, "beep_port", 0);
    uint8_t beep_offset = config_section_get_uint(sect, "beep_offset", 100);

    if ((beep_port != 0) && (beep_offset != 100))
        *beep_port |= (1 << beep_offset);
}
void reader_beep_on(config_section_t * sect) {
    volatile uint8_t * beep_port = (volatile uint8_t *) config_section_get_uint(sect, "beep_port", 0);
    uint8_t beep_offset = config_section_get_uint(sect, "beep_offset", 100);

    if ((beep_port != 0) && (beep_offset != 100))
        *beep_port &= ~(1 << beep_offset);

}

void reader_net_callback(config_section_t * conf_sect, msg_lvl2_t * net_msg) {
    if ((conf_sect == NULL) || (net_msg == NULL))
        return;
    pmd_reader_data_t reader_data;
    int rc;
    rc = pmd_reader_read_data(net_msg->data, &reader_data);

    if (rc == 0) {
        switch (reader_data.operation) {
        case PMD_READER_GREEN_LED_ON:
            reader_green_led_on(conf_sect);
            break;
        case PMD_READER_GREEN_LED_OFF:
            reader_green_led_off(conf_sect);
            break;
        case PMD_READER_BEEP_ON:
            reader_beep_on(conf_sect);
            break;
        case PMD_READER_BEEP_OFF:
            reader_beep_off(conf_sect);
            break;
        default:
            break;
        }
    }

}
// periphery control

void reader_clean_msg(void * data) {
    reader_t * reader = (reader_t *) data;
    reader->it = WB;
}

void reader_handle_data0(void * data) {
    reader_t * reader = (reader_t *) data;

    reader->msg &= ~((uint64_t) 1 << reader->it);

    if (reader->it == WB
        ) ftimer_register_func(reader_clean_msg, data, 500);

    if (reader->it == 0) {
        process_post(&process_handle_reader, reader_input, data);
        reader->it = WB;
    } else {
        reader->it--;
    }
    interrupt_register(config_section_get_uint(reader->param, "data0_offset", 8), INTERRUPT_FALL,
            reader_handle_data0, data);
}

void reader_handle_data1(void * data) {
    reader_t * reader = (reader_t *) data;

    reader->msg |= ((uint64_t) 1 << reader->it);

    if (reader->it == WB
        ) ftimer_register_func(reader_clean_msg, data, 500);

    if (reader->it == 0) {
        process_post(&process_handle_reader, reader_input, data);
        reader->it = WB;
    } else {
        reader->it--;
    }
    interrupt_register(config_section_get_uint(reader->param, "data1_offset", 8), INTERRUPT_FALL,
            reader_handle_data1, data);
}

int reader_init(config_section_t * sect) {
    if (sect == NULL
        ) return 1;
    reader_t * reader = (reader_t *) malloc(sizeof(reader_t));
    reader->param = sect;
    reader->it = WB;
    reader->msg = 0;

    volatile uint8_t * data0_ddr = (volatile uint8_t *) config_section_get_uint(reader->param, "data0_ddr", 0);
    uint8_t data0_offset = config_section_get_uint(reader->param, "data0_offset", 0);

    volatile uint8_t * data1_ddr = (volatile uint8_t *) config_section_get_uint(reader->param, "data1_ddr", 0);
    uint8_t data1_offset = config_section_get_uint(reader->param, "data1_offset", 0);

    //periphery
    volatile uint8_t * green_led_ddr = (volatile uint8_t *) config_section_get_uint(reader->param, "gled_ddr", 0);
    uint8_t green_led_offset = config_section_get_uint(reader->param, "gled_offset", 100);
    volatile uint8_t * beep_ddr = (volatile uint8_t *) config_section_get_uint(reader->param, "beep_ddr",0);
    uint8_t beep_offset = config_section_get_uint(reader->param, "beep_offset", 100);

    *data0_ddr &= ~(1 << data0_offset);
    *data1_ddr &= ~(1 << data1_offset);

    //periphery
    if ((green_led_ddr != 0) && (green_led_offset != 100)) {
        *green_led_ddr |= (1 << green_led_offset);
        reader_green_led_off(reader->param);
    }
    if ((beep_ddr != 0) && (beep_offset != 100)) {
        *beep_ddr |= (1 << beep_offset);
        reader_beep_off(reader->param);
    }

    process_start(&process_handle_reader, NULL);

    //enable interrupts
    interrupt_register(config_section_get_uint(reader->param, "data0_offset", 8), INTERRUPT_FALL,
            reader_handle_data0, (void *) reader);
    interrupt_register(config_section_get_uint(reader->param, "data1_offset", 8), INTERRUPT_FALL,
            reader_handle_data1, (void *) reader);
    return 0;
}

PROCESS_THREAD(process_handle_reader, ev, data) {
    reader_t * reader;
    bytearr_t arr;
    msg_lvl2_t msg;
    pmd_reader_data_t * msg_data;
    int i;

    PROCESS_BEGIN();
        reader_input = process_alloc_event();

        for(;;) {
            PROCESS_WAIT_EVENT();
            if (ev == reader_input) {
                reader = (reader_t *) data;
                msg_data = (pmd_reader_data_t *)malloc(sizeof(pmd_reader_data_t));

                //FIXME not using parity control bits
                reader->msg &= ~((uint64_t)1 << WB);
                reader->msg >>= 1;

                msg.meta.hw_addr = 7;//FIXME right hw_addr
                msg.meta.port = 1;//FIXME right port
                msg.meta.id = reader->param->id;
                msg.meta.is_system = 0;

                msg_data->operation = PMD_READER_SEND_MSG;
                for (i = PMD_READER_MSG_LEN - 1; i >= 0; i--) {
                    msg_data->data[i] = (uint8_t)(reader->msg >> (i * 8));
                }
                arr.len = PMD_READER_MSG_LEN + 1;
                arr.itself = (uint8_t *)malloc(arr.len);
                pmd_reader_write_data(arr, msg_data);
                msg.data = arr;

                can_net_start_sending_msg(&msg, NULL, NULL);

            }
        }

        PROCESS_END();
}
