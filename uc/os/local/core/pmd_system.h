/*
 * Copyright (c) 2011 PMDteam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is a part of PMD Subsystem.
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
