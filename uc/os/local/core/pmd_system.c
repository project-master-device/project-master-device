#include "pmd_system.h"

#include "net/can_net.h"
#include "lib/net_device.h"
#include "lib/interrupt.h"

#include <avr/io.h>

/**
 * @brief Two can_net callbacks: 0 -- for non-smb messages, 1 -- for smb messages.
 */
static can_net_recv_callback_record_t cb_records[2];
static can_net_recv_callback_record_t net_recv_cb_record;


static void non_smb_net_cb(msg_lvl2_t * msg) {
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
static void smb_net_cb(msg_lvl2_t * msg) {
    led3_blink(1, 200);
}


static void create_config() {
    config_cnf_t * cnf = config_get();

    //create led 1
    config_section_t * sect = config_cnf_create_section(cnf);
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

    sect = config_cnf_create_section(cnf);
    sect->id = 5;
    config_section_set_str(sect, "type", "reader");
    config_section_set_uint(sect, "data0_ddr", 45);
    config_section_set_uint(sect, "data0_pin", 44);
    config_section_set_uint(sect, "data0_offset", 4);
    config_section_set_uint(sect, "data1_ddr", 45);
    config_section_set_uint(sect, "data1_pin", 44);
    config_section_set_uint(sect, "gled_ddr", &DDRA);
    config_section_set_uint(sect, "gled_port", &PORTA);
    config_section_set_uint(sect, "gled_offset", PA7);
    config_section_set_uint(sect, "beep_ddr", &DDRA);
    config_section_set_uint(sect, "beep_port", &PORTA);
    config_section_set_uint(sect, "beep_offset", PA6);

    config_save();
}

int pmd_system_init() {
    config_init();
    create_config();
    if (config_open() == -1) {
        return 1;
    }

    if(can_net_init(10000000, 100) != 0) {
        return 2;
    }

    interrupt_init();

    if(net_device_init() != 0) {
        return 3;
    }

    can_net_recv_callbacks_arr_t cb_arr;
    cb_arr.records = &cb_records;
    cb_arr.len = 2;

    //Non-smb message handler
    cb_arr.records[0].check.port_min = 0;
    cb_arr.records[0].check.port_max = -1;
    cb_arr.records[0].check.id_min = 0;
    cb_arr.records[0].check.id_max = -1;
    cb_arr.records[0].check.smb_min = 0;
    cb_arr.records[0].check.smb_max = 0;
    cb_arr.records[0].callback = non_smb_net_cb;

    //smb message handler
    cb_arr.records[1].check.port_min = 0;
    cb_arr.records[1].check.port_max = -1;
    cb_arr.records[1].check.id_min = 0;
    cb_arr.records[1].check.id_max = -1;
    cb_arr.records[1].check.smb_min = 1;
    cb_arr.records[1].check.smb_max = 1;
    cb_arr.records[1].callback = smb_net_cb;

    can_net_add_callbacks(cb_arr);

    return 0;
}
