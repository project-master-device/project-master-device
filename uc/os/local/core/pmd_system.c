#include "pmd_system.h"

#include "net/can_net.h"
#include "lib/net_device.h"
#include "lib/interrupt.h"
#include "pmd_net/system.h"

#include <avr/io.h>

/**
 * @brief Two can_net callbacks: 0 -- for non-smb messages, 1 -- for smb messages.
 */
static can_net_recv_cb_record_t cb_records[2];


static void non_smb_net_cb(const msg_lvl2_t * msg, void * ctx) {
    if(msg == NULL) return;

    net_device_t * net_device = net_device_get(msg->meta.id);
    if(net_device != NULL) {
        if(net_device->net_callback != NULL) {
            net_device->net_callback(net_device->conf_sect, msg);
        }
    } else {
        //TODO ???
    }
}

/*
 * SMB messages:
 * 1. Get list of devices (with id)
 *
 * Operation modes:
 * 1. Init
 * 2. Normal
 */
/*
 * TODO: проверить сообщения:
 * 1. Мастер -> контроллер: запрос конфига [ok]
 * 2. Контроллер -> мастер: полный конфиг [ok]
 * 3. Мастер -> контроллер: удалить секцию
 * 4. Мастер -> контроллер: добавить секцию
 */

static void config_full_handler(pmd_net_system_config_data_t * cd) {
    if((cd == NULL) || (cd->config == NULL)) {
        return;
    }

    config_set(cd->config);
}

// Send full config
static void config_request_handler() {
    msg_lvl2_t msg;
    msg.meta.hw_addr = 7; //FIXME: write real address here
    msg.meta.port = 1; //FIXME: write real port here
    msg.meta.is_system = 1;
    msg.meta.id = PMD_NET_SYSTEM_CONFIG_MSG;

    pmd_net_system_config_data_t cd;
    cd.config = config_get();
    cd.operation = PMD_NET_SYSTEM_CONFIG_FULL;
    pmd_net_system_config_write_data(&(msg.data), &cd);

    can_net_start_sending_msg(&msg, NULL, NULL);
    if(msg.data.itself != NULL) {
        free(msg.data.itself);
        msg.data.len = 0;
        msg.data.itself = NULL;
    }
}

static void config_section_add_handler(pmd_net_system_config_data_t * cd) {
    if((cd == NULL) || (cd->section == NULL)) {
        return;
    }

    config_cnf_add_section(config_get(), cd->section);
}

static void config_section_del_handler(pmd_net_system_config_data_t * cd) {
    if((cd == NULL) || (cd->section == NULL)) {
        return;
    }

    config_section_t * sect = config_cnf_find_section(config_get(), cd->section->id);
    config_cnf_del_section(config_get(), sect);
}

static void smb_net_cb(const msg_lvl2_t * msg, void * ctx) {
    pmd_net_system_config_data_t cd;
    cd.config = NULL;
    cd.section = NULL;

    if(msg != NULL) {
        led2_blink(1, 200);

        switch(msg->meta.id) {
        case PMD_NET_SYSTEM_CONFIG_MSG:
            if(pmd_net_system_config_read_data(&(msg->data), &cd) == 0) {
                switch(cd.operation) {
                case PMD_NET_SYSTEM_CONFIG_FULL:
                    config_full_handler(&cd);
                    break;

                case PMD_NET_SYSTEM_CONFIG_REQUEST:
                    config_request_handler();
                    break;

                case PMD_NET_SYSTEM_CONFIG_SECTION_ADD:
                    config_section_add_handler(&cd);
                    break;

                case PMD_NET_SYSTEM_CONFIG_SECTION_DEL:
                    config_section_del_handler(&cd);
                    break;
                }
            } else {
                led1_blink(1, 1000);
            }

            break;
        }
    }

    if(cd.config != NULL) {
        free(cd.config);
        cd.config = NULL;
    }

    if(cd.section != NULL) {
        free(cd.section);
        cd.section = NULL;
    }
}


static void create_config() {
    config_destruct();
    config_construct();
    config_cnf_t * cnf = config_get();
    config_section_t * sect;

    //create led 1
    sect = config_cnf_create_section(cnf);
    sect->id = 1;
    config_section_set_str(sect, "type", "led");
    config_section_set_uint(sect, "ddr", &DDRB);
    config_section_set_uint(sect, "port", &PORTB);
    config_section_set_uint(sect, "offset", PB4);

    //create led 2
    sect = config_cnf_create_section(cnf);
    sect->id = 2;
    config_section_set_str(sect, "type", "led");
    config_section_set_uint(sect, "ddr", &DDRB);
    config_section_set_uint(sect, "port", &PORTB);
    config_section_set_uint(sect, "offset", PB5);

    //create led 3
    sect = config_cnf_create_section(cnf);
    sect->id = 3;
    config_section_set_str(sect, "type", "led");
    config_section_set_uint(sect, "ddr", &DDRB);
    config_section_set_uint(sect, "port", &PORTB);
    config_section_set_uint(sect, "offset", PB6);

//    //create external button
//    sect = config_cnf_create_section(cnf);
//    sect->id = 4;
//    config_section_set_str(sect, "type", "button");
//    config_section_set_uint(sect, "ddr", &DDRE);
//    config_section_set_uint(sect, "pin", &PINE);
//    config_section_set_uint(sect, "offset", PE7);

//    //create reader
//    sect = config_cnf_create_section(cnf);
//    sect->id = 5;
//    config_section_set_str(sect, "type", "reader");
//    config_section_set_uint(sect, "data0_ddr", 45);
//    config_section_set_uint(sect, "data0_pin", 44);
//    config_section_set_uint(sect, "data0_offset", 4);
//    config_section_set_uint(sect, "data1_ddr", 45);
//    config_section_set_uint(sect, "data1_pin", 44);
//    config_section_set_uint(sect, "data1_offset", 5);
//    config_section_set_uint(sect, "gled_ddr", &DDRA);
//    config_section_set_uint(sect, "gled_port", &PORTA);
//    config_section_set_uint(sect, "gled_offset", PA7);
//    config_section_set_uint(sect, "beep_ddr", &DDRA);
//    config_section_set_uint(sect, "beep_port", &PORTA);
//    config_section_set_uint(sect, "beep_offset", PA6);

    config_save();
}

int pmd_system_init() {
    create_config();
    if (config_open() != 0) {
        return 1;
    }

    if(can_net_init(10000000, 100) != 0) {
        return 2;
    }

    interrupt_init();

    if(net_device_init() != 0) {
        return 3;
    }

    //Non-smb message handler
    cb_records[0].check.port_min = 0;
    cb_records[0].check.port_max = -1;
    cb_records[0].check.id_min = 0;
    cb_records[0].check.id_max = -1;
    cb_records[0].check.smb_min = 0;
    cb_records[0].check.smb_max = 0;
    cb_records[0].callback = non_smb_net_cb;
    cb_records[0].cb_ctx = NULL;

    //smb message handler
    cb_records[1].check.port_min = 0;
    cb_records[1].check.port_max = -1;
    cb_records[1].check.id_min = 0;
    cb_records[1].check.id_max = -1;
    cb_records[1].check.smb_min = 1;
    cb_records[1].check.smb_max = 1;
    cb_records[1].callback = smb_net_cb;
    cb_records[1].cb_ctx = NULL;

    can_net_add_callback( &(cb_records[0]) );
    can_net_add_callback( &(cb_records[1]) );

    return 0;
}
