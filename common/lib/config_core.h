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


#ifndef CONFIG_CORE_H_
#define CONFIG_CORE_H_

#include <stdint.h>

#include "list.h"

typedef int                     config_int_t;
typedef unsigned int            config_uint_t;

#define CONFIG_OPTION_STR_TYPE          's'
#define CONFIG_OPTION_NUM_INT_TYPE      'i'
#define CONFIG_OPTION_NUM_UINT_TYPE     'I'

#define CONFIG_EOF '~'

#ifndef CONFIG_OPTION_KEY_LEN
#define CONFIG_OPTION_KEY_LEN 32
#endif

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

void config_option_construct(config_option_t * config_option);
void config_option_destruct(config_option_t * config_option);
void config_option_set_value(config_option_t * config_option, const char * value);

/**
 * @struct config_section_t
 */
typedef struct {
    void * next; ///< needed for list.

    uint8_t id; ///< section name.
    LIST_STRUCT(options); ///< options list.
} config_section_t;

config_section_t * config_section_new(uint8_t id);
void config_section_delete(config_section_t * config_section);
void config_section_construct(config_section_t * config_section, uint8_t id);
void config_section_destruct(config_section_t * config_section);
config_option_t * config_section_create_option(config_section_t * config_section);
config_option_t * config_section_find_option(config_section_t * config_section, const char * key);

/*
 * \param sect          configuration section structure pointer
 * \param key           option key in given section
 * \param defalut_val   default return value on failure
 *
 * \return              option value
 */
config_int_t config_section_get_int(config_section_t * config_section, const char * key, config_int_t default_val);
config_uint_t config_section_get_uint(config_section_t * config_section, const char * key, config_uint_t default_val);

/*
 * \param sect          configuration section structure pointer
 * \param key           option key in given section
 * \param defalut_val   default return value on failure
 * \param dest          destination value buffer
 * \param dest_size     destination buffer size
 */
void config_section_get_str(config_section_t * config_section, const char * key, const char * default_val, char * dest, unsigned int dest_size);

/*
 * \param sect  configuration section structure pointer
 * \param key   option key in given section
 * \param val   set value
 *
 * \return      1 if successful, -1 on failure
 */
int config_section_set_int(config_section_t * config_section, const char * key, config_int_t val);
int config_section_set_uint(config_section_t * config_section, const char * key, config_uint_t val);
int config_section_set_str(config_section_t * config_section, const char * key, const char * val);

/**
 * @struct config_cnf_t
 * @brief The main config structure.
 */
typedef struct  {
    LIST_STRUCT(sections); ///< list of sections.
} config_cnf_t;

config_cnf_t * config_cnf_new();
void config_cnf_delete(config_cnf_t * config);
void config_cnf_construct(config_cnf_t * config);
void config_cnf_destruct(config_cnf_t * config);
void config_cnf_copy(config_cnf_t * config, config_cnf_t * prototype);
/**
 * @brief Insert new sections into config.
 * @return
 *      0 -- successful.
 *      1 -- section with that id already exists in the config.
 *      2 -- bad config or section.
 */
int config_cnf_add_section(config_cnf_t * config, config_section_t * sect);
void config_cnf_del_section(config_cnf_t * config, config_section_t * sect);
config_section_t * config_cnf_find_section(config_cnf_t * config, uint8_t id);
config_section_t * config_cnf_create_section(config_cnf_t * config, uint8_t id);

/**
 * Parse config from string to C structure representation.
 * @return
 *      0 -- successful parse
 *      1 -- parse failed
 *      2 -- parameters problem (source string pointer or destination config pointer is NULL)
 */
int config_parse(const char * src_str, config_cnf_t * dest_config);

/**
 * Write config from C structure representation to string.
 * @return
 *      NULL-terminated C-string, which contains config.
 *      NULL pointer if emit failed.
 */
char * config_emit(config_cnf_t * src_config);

#endif /* CONFIG_CORE_H_ */
