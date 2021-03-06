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


#include "config.h"

#include <string.h>
#include <stdlib.h>


int pmd_net_sys_config_write_data(bytearr_t * dest_arr, const pmd_net_sys_config_data_t * source_data) {
    if((source_data == NULL) || (dest_arr == NULL))
        return 1;

    const char * config_str = NULL;

    switch(source_data->operation) {
    case PMD_NET_SYS_CONFIG_REQUEST:
        dest_arr->len = 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        break;

    case PMD_NET_SYS_CONFIG_FULL:
        if(source_data->config == NULL) {
            return 3;
        }

        config_str = config_emit(source_data->config);
        if(config_str == NULL) {
            return 4;
        }

        dest_arr->len = strlen(config_str) + 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        memcpy(dest_arr->itself + 1, config_str, dest_arr->len - 1);
        dest_arr->itself[0] = source_data->operation;
        break;

    case PMD_NET_SYS_CONFIG_SECTION_ADD:
        if(source_data->section == NULL) {
            return 5;
        }

        config_cnf_t tmp_cnf;
        config_cnf_construct(&tmp_cnf);
        list_add(tmp_cnf.sections, (void *)(source_data->section));

        config_str = config_emit(&tmp_cnf);
        if(config_str == NULL) {
            return 6;
        }

        dest_arr->len = strlen(config_str) + 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        memcpy(dest_arr->itself + 1, config_str, dest_arr->len - 1);
        dest_arr->itself[0] = source_data->operation;
        break;

    case PMD_NET_SYS_CONFIG_SECTION_DEL:
        if(source_data->section == NULL) {
            return 5;
        }

        dest_arr->len = 2;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        dest_arr->itself[1] = source_data->section->id;
        break;
    }

    return 0;
}

int pmd_net_sys_config_read_data(const bytearr_t * source_arr, pmd_net_sys_config_data_t * dest_data) {
    if((dest_data == NULL) || (source_arr == NULL))
        return 1;

    if((source_arr->itself == NULL) || (source_arr->len == 0))
        return 3;

    int rc;
    char buf[source_arr->len];

    switch(source_arr->itself[0]) {
    case PMD_NET_SYS_CONFIG_REQUEST:
        dest_data->operation = source_arr->itself[0];
        break;

    case PMD_NET_SYS_CONFIG_FULL:
        dest_data->config = (config_cnf_t *)malloc(sizeof(config_cnf_t));
        if(dest_data->config == NULL) {
            return 2;
        }
        config_cnf_construct(dest_data->config);

        memcpy((void *)buf, source_arr->itself + 1, source_arr->len - 1);
        buf[source_arr->len - 1] = '\0';

        rc = config_parse(buf, dest_data->config);
        if(rc != 0) {
            return 4;
        }
        dest_data->operation = source_arr->itself[0];
        break;

    case PMD_NET_SYS_CONFIG_SECTION_ADD:
        memcpy((void *)buf, source_arr->itself + 1, source_arr->len - 1);
        buf[source_arr->len - 1] = '\0';

        config_cnf_t tmp_cnf;
        config_cnf_construct(&tmp_cnf);
        rc = config_parse(buf, &tmp_cnf);
        if(rc != 0) {
            return 5;
        }
        dest_data->section = (config_section_t *)list_head(tmp_cnf.sections);
        dest_data->operation = source_arr->itself[0];
        break;

    case PMD_NET_SYS_CONFIG_SECTION_DEL:
        if(source_arr->len < 2) {
            return 3;
        }

        dest_data->section = (config_section_t *)malloc(sizeof(config_section_t));
        if(dest_data->section == NULL) {
            return 2;
        }

        dest_data->operation = source_arr->itself[0];
        dest_data->section->id = source_arr->itself[1];
        break;
    }

    return 0;
}

