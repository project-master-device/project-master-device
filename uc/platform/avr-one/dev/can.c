/*
 * Copyright (c) 2010 MikloBit
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3  Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * @(#)$
 */


#include "can.h"
#include "lib/list.h"
#include "can-drv.h"
#include "lib/malloc.h"

#include <string.h>

/**
 * \brief      Structure that holds the list of waiting can command.
 *
 *				New can command is added to can_cmd_list only when no free MOB is available
 */
LIST(can_cmd_list);

/**
 * \brief      Structure that holds pointers to currently pending can commands.
 *
 */
static can_st_cmd_t *can_mob[NB_MOB] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * \brief      Variable that holds higher 16 bit of timestamp.
 *
 */
static volatile uint16_t can_timestamp_hi = 0;

PROCESS(can_process, "CAN driver");

process_event_t can_event_message;

static void do_post_cmd(can_st_cmd_t * cmd);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(can_process, ev, data) {
    static can_st_cmd_t* cmd;
    static uint8_t mob_handle;

    PROCESS_BEGIN();

    can_event_message = process_alloc_event();

    while(1) {
        // wait on polling or timer event
        PROCESS_WAIT_EVENT() ;
        // Post event about completed commands and free the mob
        for(mob_handle = 0; mob_handle < NB_MOB; ++mob_handle) {
            cmd = can_mob[mob_handle];
            if( (cmd != 0) && (cmd->status != MOB_PENDING) ) {
                // Return command to the specified process
                if( cmd->p != 0 ) {
                    process_post(cmd->p, can_event_message, cmd);
                }
                // Delete command when no process specified
                else {
                    can_st_cmd_destruct(cmd);
                    free(cmd);
                }
                can_mob[mob_handle] = 0;
                CAN_SET_MOB(mob_handle);
                CAN_MOB_ABORT();
            }
        }

        // If waiting queue not empty, try to schedule waiting commands
        while(*can_cmd_list != 0) {
            if(can_get_mob_free() != NO_MOB) {
                cmd = list_pop(can_cmd_list);
                do_post_cmd(cmd);
            } else {
                break;
            }
        }
    }

    PROCESS_END();
}


static void can_int_handler(uint16_t source) {
    uint8_t mob_handle;
    uint8_t mob_status;
    can_st_cmd_t * cmd;

    mob_handle = 0;
    while(source != 0) {
        if(source & 1) {
            cmd = can_mob[mob_handle];
            CAN_SET_MOB(mob_handle);
            mob_status = CAN_GET_STATUS_MOB() ;
            cmd->status = mob_status;

            // RX operation finished
            if( mob_status & MOB_RX_COMPLETED_DLCW ) {
                cmd->timestamp_lo = CAN_GET_TIMESTAMP_MOB();
                cmd->timestamp_hi = can_timestamp_hi;
                cmd->dlc = CAN_GET_DLC();
                can_get_data(cmd->pt_data);
                cmd->ctrl.rtr = CAN_GET_RTR();
                if (CAN_GET_IDE()) { // if extended frame
                    cmd->ctrl.ide = 1; // extended frame
                    CAN_GET_EXT_ID(cmd->id.ext);
                } else { // else standard frame
                    cmd->ctrl.ide = 0;
                    CAN_GET_STD_ID(cmd->id.std);
                }
            }
            // CAN_MOB_ABORT();        // Free the MOB - > this will be done inside can process
            CAN_CLEAR_STATUS_MOB(); //   and reset MOb status
        }
        mob_handle++ ; source >>= 1;
    }
    process_poll(&can_process);
}


ISR(OVRIT_vect) {
    can_timestamp_hi++;
}

ISR(CANIT_vect) {
	volatile uint16_t int_source;
    volatile uint8_t canpage_save;

    canpage_save = CANPAGE;     // preserve any interrupted canpage operation
    int_source = CAN_GET_INT_MOB();
    if(int_source) {
        can_int_handler(int_source);
    }
    CAN_CLEAR_INT();
    CANPAGE = canpage_save;
}


uint8_t can_init(uint8_t mode) {
    list_init(can_cmd_list);
    if((CAN_BIT_TIMING(mode)) == 0) return 0;
    can_clear_all_mob();
    cli();
    CAN_ENABLE();
    CAN_SET_TCON(199);    // for F_CPU = 16 Mhz -> CAN timestamp will be 100uS resolution
    CAN_INT_MOB_ENABLE();
    CAN_INT_ENABLE();
    sei();
    return 1;
}


static void do_post_cmd(can_st_cmd_t * cmd) {
    uint8_t mob_handle;
    uint8_t cpt;
    uint32_t u32_temp;

    if(cmd == 0) return;

    mob_handle = can_get_mob_free();
    if (mob_handle == NO_MOB) { // add command to wait queue
        cmd->status = MOB_NOT_COMPLETED ;
        list_add(can_cmd_list, cmd);
    } else { // add command to table of pending command
        can_mob[mob_handle] = cmd;
        cmd->status = MOB_PENDING;
        CAN_SET_MOB(mob_handle);
        CAN_CLEAR_MOB();

        switch(cmd->cmd) {
            case CAN_CMD_TX:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext);}
                else              { CAN_SET_STD_ID(cmd->id.std);}
                for (cpt=0;cpt<cmd->dlc;cpt++) CANMSG = *(cmd->pt_data + cpt);
                if (cmd->ctrl.rtr) CAN_SET_RTR();
                else CAN_CLEAR_RTR();
                CAN_SET_DLC(cmd->dlc);
                CAN_CONFIG_TX();
                break;

            case CAN_CMD_TX_DATA:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext);}
                else              { CAN_SET_STD_ID(cmd->id.std);}
                for (cpt=0;cpt<cmd->dlc;cpt++) CANMSG = *(cmd->pt_data + cpt);
                cmd->ctrl.rtr=0; CAN_CLEAR_RTR();
                CAN_SET_DLC(cmd->dlc);
                CAN_CONFIG_TX();
                break;

            case CAN_CMD_TX_REMOTE:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext);}
                else              { CAN_SET_STD_ID(cmd->id.std);}
                cmd->ctrl.rtr=1; CAN_SET_RTR();
                CAN_SET_DLC(cmd->dlc);
                CAN_CONFIG_TX();
                break;

            case CAN_CMD_RX:
                u32_temp=0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_DLC(cmd->dlc);
                CAN_CLEAR_RTRMSK();
                CAN_CLEAR_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_RX_DATA:
                u32_temp=0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=0; CAN_SET_RTRMSK(); CAN_CLEAR_RTR();
                CAN_CLEAR_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_RX_REMOTE:
                u32_temp=0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=1; CAN_SET_RTRMSK(); CAN_SET_RTR();
                CAN_CLEAR_RPLV();
                CAN_CLEAR_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_RX_MASKED:
                if (cmd->ctrl.ide) {
                    CAN_SET_EXT_ID(cmd->id.ext);
                    CAN_SET_IDEMSK();
                    CAN_SET_EXT_MSK(cmd->msk);
                } else {
                    CAN_SET_STD_ID(cmd->id.std);
                    CAN_CLEAR_IDEMSK();
                    CAN_SET_STD_MSK(cmd->msk);
                }

                CAN_SET_DLC(cmd->dlc);
                CAN_CLEAR_RTRMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_RX_DATA_MASKED:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext); CAN_SET_IDEMSK();}
                else              { CAN_SET_STD_ID(cmd->id.std); CAN_CLEAR_IDEMSK();}
                // u32_temp=~0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_EXT_MSK(cmd->msk);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=0; CAN_SET_RTRMSK(); CAN_CLEAR_RTR();
                // CAN_SET_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_RX_REMOTE_MASKED:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext);}
                else              { CAN_SET_STD_ID(cmd->id.std);}
                //u32_temp=~0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_EXT_MSK(cmd->msk);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=1; CAN_SET_RTRMSK(); CAN_SET_RTR();
                CAN_CLEAR_RPLV();
                CAN_SET_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_REPLY:
                for (cpt=0;cpt<cmd->dlc;cpt++) CANMSG = *(cmd->pt_data + cpt);
                u32_temp=0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=1; CAN_SET_RTRMSK(); CAN_SET_RTR();
                CAN_SET_RPLV();
                CAN_CLEAR_IDEMSK();
                CAN_CONFIG_RX();
                break;

            case CAN_CMD_REPLY_MASKED:
                if (cmd->ctrl.ide){ CAN_SET_EXT_ID(cmd->id.ext);}
                else              { CAN_SET_STD_ID(cmd->id.std);}
                for (cpt=0;cpt<cmd->dlc;cpt++) CANMSG = *(cmd->pt_data + cpt);
                // u32_temp=~0; CAN_SET_EXT_MSK(u32_temp);
                CAN_SET_EXT_MSK(cmd->msk);
                CAN_SET_DLC(cmd->dlc);
                cmd->ctrl.rtr=1; CAN_SET_RTRMSK(); CAN_SET_RTR();
                CAN_SET_RPLV();
                CAN_SET_IDEMSK();
                CAN_CONFIG_RX();
                break;

            default:
                // case CMD_NONE or not implemented command
                cmd->status = STATUS_CLEARED;
                break;
        } // switch
    }
}

can_st_cmd_t * can_post_cmd(const can_st_cmd_t * cmd) {
	can_st_cmd_t * new_cmd = 0;

	if(cmd == 0) return 0;

	new_cmd = (can_st_cmd_t *) malloc(sizeof(can_st_cmd_t));
	can_st_cmd_construct(new_cmd, cmd);

	do_post_cmd(new_cmd);

    return new_cmd;
}

void can_abort_cmd(can_st_cmd_t * cmd) {
    uint8_t cpt;

    if(cmd == 0) return;

    if( cmd->next != 0 ) {
        // remove command from waiting queue
        list_remove(can_cmd_list, cmd);
    } else {
        // remove from pending table
        for(cpt = 0; cpt < NB_MOB; cpt ++) {
            if( cmd == can_mob[cpt] )  {
                can_mob[cpt] = 0;
                CAN_SET_MOB(cpt);
                CAN_MOB_ABORT();
                break;
            }
        }
    }
}

void can_st_cmd_construct(can_st_cmd_t * cmd, const can_st_cmd_t * prototype) {
    if((cmd == 0) || (prototype == 0)) {
        return;
    }

    cmd->next = prototype->next;
    cmd->cmd = prototype->cmd;
    cmd->id = prototype->id;
    cmd->msk = prototype->msk;

    if(prototype->pt_data == 0) {
        cmd->pt_data = 0;
        cmd->dlc = 0;
    } else {
        cmd->dlc = prototype->dlc;
        cmd->pt_data = (uint8_t *) malloc(sizeof(uint8_t) * cmd->dlc);
        memcpy(cmd->pt_data, prototype->pt_data, cmd->dlc);
    }

    cmd->status = prototype->status;
    cmd->ctrl = prototype->ctrl;
    cmd->timestamp_lo = prototype->timestamp_lo;
    cmd->timestamp_hi = prototype->timestamp_hi;
    cmd->p = prototype->p;
}

void can_st_cmd_destruct(can_st_cmd_t * cmd) {
    if(cmd == 0) return;

    if(cmd->pt_data != 0) {
        free(cmd->pt_data);
    }
}
