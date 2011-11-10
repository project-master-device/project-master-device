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


#ifndef PMD_LED_H_
#define PMD_LED_H_

#include <stdint.h>
#include "../../lib/bytearray.h"

#define PMD_NET_LED_ON      1
#define PMD_NET_LED_OFF     2
#define PMD_NET_LED_TOGGLE  3

typedef struct {
    uint8_t operation;
} pmd_net_led_data_t;

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- unknown operation
 */
int pmd_net_led_write_data(bytearr_t * dest_arr, const pmd_net_led_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      3 -- unknown operation
 *      4 -- bad source data
 */
int pmd_net_led_read_data(const bytearr_t * source_arr, pmd_net_led_data_t * dest_data);

#endif /* PMD_LED_H_ */
