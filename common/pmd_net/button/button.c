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


#include "button.h"
#include <stddef.h>
#include <stdlib.h>

int pmd_net_button_write_data(bytearr_t * dest_arr, const pmd_net_button_data_t * source_data) {
    if((dest_arr == NULL) || (source_data == NULL))
        return 1;

    switch(source_data->operation) {
    case PMD_NET_BUTTON_DOWN:
    case PMD_NET_BUTTON_UP:
        dest_arr->len = 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        break;

    default:
        return 3;
        break;
    }

    return 0;
}

int pmd_net_button_read_data(const bytearr_t * source_arr, pmd_net_button_data_t * dest_data) {
    if((source_arr == NULL) || (dest_data == NULL))
        return 1;

    if((source_arr->itself == NULL) || (source_arr->len == 0))
        return 4;

    switch(source_arr->itself[0]) {
    case PMD_NET_BUTTON_DOWN:
    case PMD_NET_BUTTON_UP:
        dest_data->operation = source_arr->itself[0];
        break;

    default:
        return 3;
        break;
    }

    return 0;
}
