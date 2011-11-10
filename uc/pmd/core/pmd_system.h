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


/**
 * @file pmd_system.h
 * @brief The main pmd system.
 */

#ifndef PMD_SYSTEM_H_
#define PMD_SYSTEM_H_

#include "contiki-conf.h"

#include "net/can_net.h"

/**
 * @brief Initialize whole pmd system.
 * Initializes config, interrupts, network, network devices.
 * @return: return codes:
 *      0 -- successful,
 *      1 -- config problem,
 *      2 -- can initialization network problem,
 *      3 -- network devices initialization failed.
 */
int pmd_system_init();

/**
 * @brief Send message over network.
 */
void pmd_system_send_message(const msg_lvl2_t* msg, can_net_send_callback_t send_callback, void* cb_ctx);

/**
 * @brief Platform specific functions to turn on/off controller board identification.
 */
void pmd_system_platform_identification_on();
void pmd_system_platform_identification_off();

#endif /* PMD_SYSTEM_H_ */
