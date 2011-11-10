/*
 * Copyright (c) 2010-2011 Sergey Gnuskov, Timofey Kirillov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include "pmd_system.h"

#include "net/can_net.h"
#include "lib/net_device.h"
#include "lib/interrupt.h"
#include "lib/ftimer.h"
#include "pmd_net/system/system_ids.h"
#include "pmd_net/system/heartbeat.h"
#include "pmd_net/system/config.h"
#include "pmd_net/system/set_op.h"

#include <avr/io.h>

/**
 * @brief Two can_net callbacks: 0 -- for non-smb messages, 1 -- for smb messages.
 */
static can_net_recv_cb_record_t cb_records[2];

typedef enum {
    OPERATION_MODE_INIT,
    OPERATION_MODE_NORMAL,
    OPERATION_MODE_CONFIGURATION
} operation_mode_t;

/**
 * @brief Keeps current controller operation mode.
 */
static operation_mode_t operation_mode = OPERATION_MODE_INIT;
static msg_lvl2_t heartbeat_msg;

static void non_smb_net_cb(const msg_lvl2_t * msg, void * ctx) {
    if(msg == NULL) return;

    net_device_t * net_device = NULL;

    switch(operation_mode) {
    case OPERATION_MODE_INIT:
        break;

    case OPERATION_MODE_NORMAL:
    case OPERATION_MODE_CONFIGURATION:
        net_device = net_device_get(msg->meta.id);
        if((net_device != NULL) && (net_device->net_callback != NULL)) {
            net_device->net_callback(net_device->conf_sect, msg);
        }
        break;

    default:
        break;
    }
}

static void config_full_handler(pmd_net_sys_config_data_t * cd) {
    if((cd == NULL) || (cd->config == NULL)) {
        return;
    }

    interrupt_disable();

    interrupt_init();

    config_set(cd->config);
    config_save();
    net_device_terminate();
    net_device_init();

    interrupt_enable();
}

// Send full config
static void config_request_handler() {
    msg_lvl2_t msg;
    msg.meta.hw_addr = 7; //FIXME: write real address here
    msg.meta.port = 1; //FIXME: write real port here
    msg.meta.is_system = 1;
    msg.meta.id = PMD_NET_SYS_CONFIG;
    msg.data.len = 0;
    msg.data.itself = NULL;

    pmd_net_sys_config_data_t cd;
    cd.config = config_get();
    cd.operation = PMD_NET_SYS_CONFIG_FULL;
    pmd_net_sys_config_write_data(&(msg.data), &cd);

    can_net_start_sending_msg(&msg, NULL, NULL);
    if(msg.data.itself != NULL) {
        free(msg.data.itself);
        msg.data.len = 0;
        msg.data.itself = NULL;
    }
}

static void config_section_add_handler(pmd_net_sys_config_data_t * cd) {
    if((cd == NULL) || (cd->section == NULL)) {
        return;
    }

    interrupt_disable();

    if(config_cnf_add_section(config_get(), cd->section) == 0) {
        cd->section = NULL;

        interrupt_init();
        config_save();
        net_device_terminate();
        net_device_init();
    }

    interrupt_enable();
}

static void config_section_del_handler(pmd_net_sys_config_data_t * cd) {
    if((cd == NULL) || (cd->section == NULL)) {
        return;
    }

    interrupt_disable();

    config_section_t * sect = config_cnf_find_section(config_get(), cd->section->id);
    if(sect != NULL) {
        interrupt_init();

        config_cnf_del_section(config_get(), sect);
        config_save();
        net_device_terminate();
        net_device_init();
    }

    interrupt_enable();
}

static void system_config_handler(const msg_lvl2_t * msg) {
    pmd_net_sys_config_data_t cd;
    cd.config = NULL;
    cd.section = NULL;

    if(pmd_net_sys_config_read_data(&(msg->data), &cd) == 0) {
        switch(cd.operation) {
        case PMD_NET_SYS_CONFIG_FULL:
            led1_blink(4, 50);
            config_full_handler(&cd);
            break;

        case PMD_NET_SYS_CONFIG_REQUEST:
            led2_blink(1, 100);
            config_request_handler();
            break;

        case PMD_NET_SYS_CONFIG_SECTION_ADD:
            led1_blink(1, 50);
            config_section_add_handler(&cd);
            break;

        case PMD_NET_SYS_CONFIG_SECTION_DEL:
            led1_blink(2, 50);
            config_section_del_handler(&cd);
            break;
        }
    }

    if(cd.config != NULL) {
        free(cd.config);
        cd.config = NULL;
    }
}

static void system_set_op_handler(const msg_lvl2_t * msg) {
    pmd_net_sys_set_op_data_t opd;

    if(pmd_net_sys_set_op_read_data(&(msg->data), &opd) == 0) {
        switch(opd.operation) {
        case PMD_NET_SYS_SET_OP_INIT:
            operation_mode = OPERATION_MODE_INIT;
            pmd_system_platform_identification_off();
            break;

        case PMD_NET_SYS_SET_OP_NORMAL:
            operation_mode = OPERATION_MODE_NORMAL;
            pmd_system_platform_identification_off();
            break;

        case PMD_NET_SYS_SET_OP_CONFIGURATION:
            operation_mode = OPERATION_MODE_CONFIGURATION;
            pmd_system_platform_identification_on();
            break;

        default:
            break;
        }
    }
}

static void smb_net_cb(const msg_lvl2_t * msg, void * ctx) {
    if(msg != NULL) {
        switch(msg->meta.id) {
        case PMD_NET_SYS_CONFIG:
            system_config_handler(msg);
            break;

        case PMD_NET_SYS_SET_OP:
            system_set_op_handler(msg);
            break;

        case PMD_NET_SYS_HEARTBEAT:
            break;
        }
    }
}


static void create_config() {
    config_destruct();
    config_construct();
    config_cnf_t * cnf = config_get();
    config_section_t * sect;

    //create led 1
    sect = config_cnf_create_section(cnf, 1);
    config_section_set_str(sect, "type", "led");
    config_section_set_uint(sect, "ddr", &DDRB);
    config_section_set_uint(sect, "port", &PORTB);
    config_section_set_uint(sect, "offset", PB4);

//    //create led 2
//    sect = config_cnf_create_section(cnf, 2);
//    config_section_set_str(sect, "type", "led");
//    config_section_set_uint(sect, "ddr", &DDRB);
//    config_section_set_uint(sect, "port", &PORTB);
//    config_section_set_uint(sect, "offset", PB5);

//    //create led 3
//    sect = config_cnf_create_section(cnf, 3);
//    config_section_set_str(sect, "type", "led");
//    config_section_set_uint(sect, "ddr", &DDRB);
//    config_section_set_uint(sect, "port", &PORTB);
//    config_section_set_uint(sect, "offset", PB6);

//    //create external button
//    sect = config_cnf_create_section(cnf, 4);
//    config_section_set_str(sect, "type", "button");
//    config_section_set_uint(sect, "ddr", &DDRE);
//    config_section_set_uint(sect, "pin", &PINE);
//    config_section_set_uint(sect, "offset", PE7);

//    //create reader
//    sect = config_cnf_create_section(cnf, 5);
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

static void heartbeat_msg_init(msg_lvl2_t * msg) {
    if(msg != NULL) {
        msg->meta.hw_addr = 7; //FIXME: use real address
        msg->meta.port = 14; // broadcast port
        msg->meta.id = PMD_NET_SYS_HEARTBEAT;
        msg->meta.is_system = 1;
        msg->data.itself = NULL;
        msg->data.len = 0;

        pmd_net_sys_heartbeat_write_data(&(msg->data));
    }
}

static void send_heartbeat(void * ctx) {
    can_net_start_sending_msg(&heartbeat_msg, NULL, NULL);
    ftimer_register_func(send_heartbeat, NULL, 1000);
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

    operation_mode = OPERATION_MODE_NORMAL;

    heartbeat_msg_init(&heartbeat_msg);
    ftimer_register_func(send_heartbeat, NULL, 1000);

    return 0;
}

void pmd_system_send_message(const msg_lvl2_t* msg, can_net_send_callback_t send_callback, void* cb_ctx) {
    switch(operation_mode) {
    case OPERATION_MODE_INIT:
        break;

    case OPERATION_MODE_NORMAL:
    case OPERATION_MODE_CONFIGURATION:
        can_net_start_sending_msg(msg, send_callback, cb_ctx);
        break;

    default:
        break;
    }
}
