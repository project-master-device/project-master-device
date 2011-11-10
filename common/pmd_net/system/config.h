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


#ifndef PMD_NET_SYS_CONFIG_H_
#define PMD_NET_SYS_CONFIG_H_

#include "../../lib/config_core.h"
#include "../../lib/bytearray.h"


typedef struct {
    uint8_t operation;
    config_cnf_t * config;
    config_section_t * section;
} pmd_net_sys_config_data_t;

// Config message operations
#define PMD_NET_SYS_CONFIG_REQUEST       1
#define PMD_NET_SYS_CONFIG_FULL          2
#define PMD_NET_SYS_CONFIG_SECTION_ADD   3
#define PMD_NET_SYS_CONFIG_SECTION_DEL   4

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- config full command error: config pointer is NULL
 *      4 -- config full command error: failed to write config
 *      5 -- config section operation command (add/del) error: section pointer is NULL
 *      6 -- config section add command error: failed to write section
 */
int pmd_net_sys_config_write_data(bytearr_t * dest_arr, const pmd_net_sys_config_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source array or destination data pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- bad source data
 *      4 -- config full command error: parsing failed
 *      5 -- config section add command error: parsing failed
 */
int pmd_net_sys_config_read_data(const bytearr_t * source_arr, pmd_net_sys_config_data_t * dest_data);

#endif /* PMD_NET_SYS_CONFIG_H_ */
