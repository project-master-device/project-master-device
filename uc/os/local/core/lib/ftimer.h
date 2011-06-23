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
 * @file ftimer.h
 * @brief A tool to call function after given time (by timer).
 *
 * You should start process <b>ftimer_process</b> to work with it.
 */
#ifndef PCALL_H_
#define PCALL_H_
#include "sys/process.h"
#include "sys/etimer.h"
#include <stdint.h>

PROCESS_NAME(ftimer_process);

/**
 * @brief Add function to list of ftimer.
 *
 * This function will be called after given time. Needed to correct wait.
 *
 * @param func function to call
 * @param context context that will be given to func
 * @param tq how much time to wait before call (in ms)
 *
 * @return id or -1 if list of ftimer is full
 */
int ftimer_register_func(void (*func)(void *), void * context, uint16_t ms);

/**
 * @brief Remove function from list of ftimer.
 *
 * @param id id of function to remove
 */
void ftimer_unregister_func(uint8_t id);

#endif /* PCALL_H_ */
