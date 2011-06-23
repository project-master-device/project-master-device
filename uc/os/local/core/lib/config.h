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

#include "lib/list.h"
#include "contiki-conf.h"

typedef int                     config_int_t;
typedef unsigned int            config_uint_t;

/**
 * @struct config_option_t
 */
typedef struct {
    void * next; ///< needed for list.

    char key[CONFIG_OPTION_KEY_LEN]; ///< option key (name).
    void * value; ///< option val
    unsigned int size; ///< size of value.
    char type; ///< type of value.
} config_option_t;

void config_option_construct(config_option_t *);
void config_option_destruct(config_option_t *);
void config_option_set_value(config_option_t *, const char *);

/**
 * @struct config_section_t
 */
typedef struct {
    void * next; ///< needed for list.

    uint8_t id; ///< section name -- device id.
    LIST_STRUCT(options); ///< options list.
} config_section_t;

void config_section_construct(config_section_t *);
void config_section_destruct(config_section_t *);
config_option_t * config_section_create_option(config_section_t *);
config_option_t * config_section_find_option(config_section_t *, const char *);

config_int_t config_section_get_int(config_section_t * sect, const char * key, config_int_t default_val);
config_uint_t config_section_get_uint(config_section_t * sect, const char * key, config_uint_t default_val);
void config_section_get_str(config_section_t * sect, const char * key, const char * default_val, char * dest, unsigned int dest_size);
int config_section_set_int(config_section_t * sect, const char * key, config_int_t val);
int config_section_set_uint(config_section_t * sect, const char * key, config_uint_t val);
int config_section_set_str(config_section_t * sect, const char * key, const char * val);

/**
 * @struct config_cnf_t
 * @brief The main config structure.
 */
typedef struct  {
    LIST_STRUCT(sections); ///< list of sections.
} config_cnf_t;

void config_cnf_construct(config_cnf_t *);
void config_cnf_destruct(config_cnf_t *);
config_section_t * config_cnf_create_section(config_cnf_t *);

/**
 * @brief Initialize config structure.
 * Should be called befor using config.
 */
void config_init();

/**
 * @brief Load config from eeprom to memory.
 * Should be called after config_init.
 *
 * @return 1 if successful and -1 on failure.
 */
int config_open();

/**
 * @brief Clean configuration from memory.
 */
void config_close();
/**
 * @brief Save configuration from memory to eeprom.
 *
 * @return 1 if successful and -1 on failure.
 */
int config_save();

/**
 * @brief Get pointer to config.
 *
 * @return pointer to config.
 */
config_cnf_t * config_get();

#ifndef NDEBUG
void config_print();
#endif

#endif /* CONFIG_H_ */
