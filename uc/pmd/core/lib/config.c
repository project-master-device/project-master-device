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


#include "config.h"
#include "cfs/cfs.h"

#include <string.h>

#define CONFIG_FILENAME NULL

static config_cnf_t config;

void config_construct() {
    config_cnf_construct(&config);
}

int config_open() {
    int filemode = CFS_READ;
    int fd = cfs_open(CONFIG_FILENAME, filemode);

    if(fd == -1) {
        cfs_close(fd);
        fd = cfs_open(CONFIG_FILENAME, filemode);
        if(fd == -1) {
            return 1;
        }
    }

    config_destruct();
    config_construct();

    char config_buf[256];
    char * config_buf_it = config_buf;
    char * config_buf_end = config_buf + 256;

    cfs_read(fd, (void *)config_buf_it, 1);
    while( ((*config_buf_it) != CONFIG_EOF) && (config_buf_it != config_buf_end) ) {
        config_buf_it += 1;
        cfs_read(fd, config_buf_it, 1);
    }
    cfs_close(fd);

    config_buf_it += 1;
    if(config_buf_it >= config_buf_end) {
        //small buffer
        return 2;
    }
    *config_buf_it = '\0';

    if(config_parse(config_buf, &config) == 1) {
        return 3;
    }

    return 0;
}

void config_destruct() {
    config_cnf_destruct(&config);
}

int config_save() {
    int filemode = CFS_WRITE;
    int fd = cfs_open(CONFIG_FILENAME, filemode);

    if(fd == -1) {
        cfs_close(fd);
        fd = cfs_open(CONFIG_FILENAME, filemode);
        if(fd == -1) {
            return 1;
        }
    }

    const char * config_str = config_emit(&config);

    if(config_str != NULL) {
        cfs_write(fd, (void *)config_str, strlen(config_str));
        cfs_close(fd);
    } else {
        cfs_close(fd);
        return 2;
    }

    return 0;
}

config_cnf_t * config_get() {
    return &config;
}

void config_set(config_cnf_t * cnf) {
    if(cnf != NULL) {
        config_destruct();
        config_cnf_copy(&config, cnf);
    }
}
