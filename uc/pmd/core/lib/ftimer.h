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
