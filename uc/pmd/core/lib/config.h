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
 * @file config.h
 * @brief Eeprom configuration library.
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include "contiki-conf.h"
#include "common_lib/config_core.h"

/**
 * @brief Initialize config structure.
 * Should be called if config was destructed manually.
 */
void config_construct();

/**
 * @brief Load config from eeprom to memory.
 * Automatically construct config object.
 *
 * @return
 *      0 -- successful
 *      1 -- filesystem problem
 *      2 -- config length too big
 *      3 -- config parse failed
 */
int config_open();

/**
 * @brief Clean configuration from memory.
 */
void config_destruct();
/**
 * @brief Save configuration from memory to eeprom.
 *
 * @return
 *      0 -- successful
 *      1 -- filesystem problem
 *      2 -- config emit failed
 */
int config_save();

/**
 * @brief Get pointer to config.
 *
 * @return pointer to config.
 */
config_cnf_t * config_get();

/**
 * @brief Set new config.
 *      Copies cnf struct into internal variable.
 */
void config_set(config_cnf_t * cnf);

#endif /* CONFIG_H_ */
