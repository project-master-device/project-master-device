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
