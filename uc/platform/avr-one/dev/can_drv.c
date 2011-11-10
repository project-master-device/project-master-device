/*
 * Copyright (c) 2010-2011 Timofey Kirillov
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


#include "contiki.h"
#include "net/can_drv.h"
#include "lib/list.h"
#include "can_lowlevel.h"
#include "lib/malloc.h"
#include "lib/ftimer.h"

#include <stddef.h>
#include <string.h>


typedef enum {
    RX,
    TX
} can_cmd_type_t;

typedef struct {
    void * next;

    can_cmd_type_t type;
    uint8_t status;
    uint8_t ftimer_id;

    can_frame_t * frame;
    void * context;
} can_cmd_t;


static int create_recv_buf();
static int init_recv_buffers();
static void post_cmd(can_cmd_t * cmd);
static void int_handler(uint16_t source);
static void send_timeout_handler(void * cmd);

static can_send_callback_t can_send_callback;
static can_recv_callback_t can_recv_callback;

#ifndef CAN_RECV_BUF_MAX
#error CAN_RECV_BUF_MAX not defined for the can driver
#endif

static const uint8_t can_recv_buf_max = CAN_RECV_BUF_MAX;
static uint32_t can_send_timeout_ms = 0;
static can_mask_t can_masked_id = 0;
static can_mask_t can_mask = 0;

/**
 * @brief Structure that holds the list of waiting can command.
 * New can command is added to this list only when no free MOB is available
 */
LIST(can_cmd_list);

/**
 * @brief Array that holds pointers to currently pending can commands.
 */
static can_cmd_t * can_mob[NB_MOB] = {NULL, NULL, NULL, NULL, NULL,
                                      NULL, NULL, NULL, NULL, NULL,
                                      NULL, NULL, NULL, NULL, NULL};

PROCESS(can_drv_process, "CAN driver process");

PROCESS_THREAD(can_drv_process, ev, data) {
    static can_cmd_t * cmd;
    static uint8_t mob_handle;
    uint8_t do_create_recv_buf = 0;

    PROCESS_BEGIN();

    for(;;) {
        // wait on polling or timer event
        PROCESS_WAIT_EVENT();

        // Post event about completed commands and free the mob
        for(mob_handle = 0; mob_handle < NB_MOB; ++mob_handle) {
            cmd = can_mob[mob_handle];
            do_create_recv_buf = 0;

            if(cmd != NULL) {
                if(cmd->status != MOB_PENDING) {
                    if(cmd->status & MOB_TX_COMPLETED) {
                        ftimer_unregister_func(cmd->ftimer_id);
                        can_send_callback(CAN_DRV_RC_NORM, cmd->frame, cmd->context);
                    } else if(cmd->status & MOB_RX_COMPLETED) {
                        can_recv_callback(cmd->frame);
                        do_create_recv_buf = 1;
                    }
                    can_mob[mob_handle] = NULL;
                    free(cmd);
                    CAN_SET_MOB(mob_handle);
                    CAN_MOB_ABORT();
                }
            }

            if(do_create_recv_buf) {
                create_recv_buf();
            }
        }

        // If waiting queue not empty, try to schedule waiting commands
        while( list_head(can_cmd_list) != 0 ) {
            if(can_get_mob_free() != NO_MOB) {
                cmd = list_pop(can_cmd_list);
                post_cmd(cmd);
            } else {
                break;
            }
        }
    }

    PROCESS_END();
}

static void int_handler(uint16_t source) {
    uint8_t mob_handle;
    can_cmd_t * cmd;

    mob_handle = 0;
    while(source != 0) {
        if(source & 1) {
            cmd = can_mob[mob_handle];

            if(cmd != NULL) {
                CAN_SET_MOB(mob_handle);
                cmd->status = CAN_GET_STATUS_MOB() ;

                // RX operation finished
                if(cmd->status & MOB_RX_COMPLETED_DLCW) {
                    cmd->frame->data.len = CAN_GET_DLC();
                    can_get_data(cmd->frame->data.itself);
                    cmd->frame->inf.rtr = CAN_GET_RTR();

                    if (CAN_GET_IDE()) {
                        cmd->frame->inf.ide = 1;
                        CAN_GET_EXT_ID(cmd->frame->inf.id);
                    } else {
                        cmd->frame->inf.ide = 0;
                        CAN_GET_STD_ID(cmd->frame->inf.id);
                    }
                }
            }

            // reset MOb status
            CAN_CLEAR_STATUS_MOB();
        }

        ++mob_handle;
        source >>= 1;
    }
    process_poll(&can_drv_process);
}

ISR(OVRIT_vect) {}

ISR(CANIT_vect) {
    volatile uint16_t int_source;
    volatile uint8_t canpage_save;

    // preserve any interrupted canpage operation
    canpage_save = CANPAGE;
    int_source = CAN_GET_INT_MOB();
    if(int_source) {
        int_handler(int_source);
    }
    CAN_CLEAR_INT();
    CANPAGE = canpage_save;
}

static void post_cmd(can_cmd_t * cmd) {
    uint8_t mob_handle;
    uint8_t i;

    if(cmd == NULL) return;

    mob_handle = can_get_mob_free();

    // add command to wait queue if there are no free mobs
    if(mob_handle == NO_MOB) {
        cmd->status = MOB_NOT_COMPLETED;
        list_add(can_cmd_list, cmd);
    }
    // add command to table of pending command otherwise
    else {
        can_mob[mob_handle] = cmd;
        cmd->status = MOB_PENDING;
        CAN_SET_MOB(mob_handle);
        CAN_CLEAR_MOB();

        switch(cmd->type) {
            case TX:
                if(cmd->frame->inf.ide) {
                    CAN_SET_EXT_ID(cmd->frame->inf.id);
                } else {
                    CAN_SET_STD_ID(cmd->frame->inf.id);
                }

                for(i = 0; i < cmd->frame->data.len; ++i) {
                    CANMSG = cmd->frame->data.itself[i];
                }

                if(cmd->frame->inf.rtr) {
                    CAN_SET_RTR();
                } else {
                    CAN_CLEAR_RTR();
                }
                CAN_SET_DLC(cmd->frame->data.len);
                CAN_CONFIG_TX();
                break;

            case RX:
                if (cmd->frame->inf.ide) {
                    CAN_SET_EXT_ID(can_masked_id);
                    CAN_SET_IDEMSK();
                    CAN_SET_EXT_MSK(can_mask);
                } else {
                    CAN_SET_STD_ID(can_masked_id);
                    CAN_CLEAR_IDEMSK();
                    CAN_SET_STD_MSK(can_mask);
                }
                CAN_SET_DLC(cmd->frame->data.len);
                CAN_CLEAR_RTRMSK();
                CAN_CONFIG_RX();
                break;

            default:
                cmd->status = STATUS_CLEARED;
                break;
        }
    }
}

static int create_recv_buf() {
    can_cmd_t * cmd;

    cmd = (can_cmd_t *) malloc(sizeof(can_cmd_t));
    if(cmd != NULL) {
        cmd->frame = (can_frame_t *) malloc(sizeof(can_frame_t));
        if(cmd->frame != NULL) {
            cmd->type = RX;
            cmd->context = 0;
            cmd->next = 0;
            cmd->status = MOB_DISABLE;

            post_cmd(cmd);
        } else {
            free(cmd);
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

static int init_recv_buffers() {
    uint8_t i;

    for(i = 0; i < can_recv_buf_max; ++i) {
        if(create_recv_buf() == -1) {
            return -1;
        }
    }

    return 0;
}

static void abort_cmd(can_cmd_t * cmd) {
    uint8_t cpt;

    if(cmd == NULL) return;

    if(cmd->next != NULL) {
        // remove command from waiting queue
        list_remove(can_cmd_list, cmd);
    } else {
        // remove from pending table
        for(cpt = 0; cpt < NB_MOB; ++cpt) {
            if(cmd == can_mob[cpt])  {
                can_mob[cpt] = NULL;
                CAN_SET_MOB(cpt);
                CAN_MOB_ABORT();
                break;
            }
        }
    }
}

static void send_timeout_handler(void * cmd) {
    if(cmd == NULL) return;

    abort_cmd((can_cmd_t *)cmd);
    can_send_callback(CAN_DRV_RC_TIMEOUT, ((can_cmd_t *)cmd)->frame, ((can_cmd_t *)cmd)->context);

    free(cmd);
}

int can_set_speed(can_speed_t can_speed) {
    return can_set_baudrate(can_speed);
}

int can_init(can_speed_t can_speed, can_send_callback_t send_callback, can_recv_callback_t recv_callback,
        uint32_t send_timeout_real_us, can_mask_t masked_id, can_mask_t mask) {

    if((send_callback == NULL) || (recv_callback == NULL)) {
        return 1;
    }

    cli();
    can_send_callback = send_callback;
    can_recv_callback = recv_callback;
    can_send_timeout_ms = send_timeout_real_us / 1000;

    can_masked_id = masked_id;
    can_mask = mask;

    if(can_set_speed(can_speed) == -1) return -1;

    list_init(can_cmd_list);
    can_clear_all_mob();

    CAN_ENABLE();
    CAN_SET_TCON(199); // FIXME for F_CPU = 16 Mhz -> CAN timestamp will be 100uS resolution
    CAN_INT_MOB_ENABLE();
    CAN_INT_ENABLE();
    sei();

    if(init_recv_buffers() == -1) {
        return -2;
    }

    return 0;
}

int can_send(can_frame_t * can_frame, void * context) {
    can_cmd_t * cmd;

    if(can_frame == NULL) return -1;

    cmd = (can_cmd_t *) malloc(sizeof(can_cmd_t));
    if(cmd != NULL) {
        cmd->frame = can_frame;
        cmd->context = context;
        cmd->type = TX;
        cmd->next = NULL;
        cmd->status = MOB_DISABLE;

        post_cmd(cmd);
        cmd->ftimer_id = ftimer_register_func(send_timeout_handler, cmd, can_send_timeout_ms);
    } else {
        return -2;
    }

    return 0;
}
