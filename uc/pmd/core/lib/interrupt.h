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
 * @file interrupt.h
 * @brief A lib for managing interrupts.
 */
#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include <stdint.h>


typedef enum {
    INTERRUPT_LOWLEVEL,
    INTERRUPT_ANY,
    INTERRUPT_FALL,
    INTERRUPT_RISE
} interrupt_type_t;

/**
 * @brief Enable interrupts.
 */
void interrupt_init();

/**
 * @brief Global enable/disable interrupts.
 */
void interrupt_enable();
void interrupt_disable();

uint8_t interrupt_register(uint8_t i, interrupt_type_t type, void (*callb)(void *), void * data);

#endif /* INTERRUPT_H_ */
