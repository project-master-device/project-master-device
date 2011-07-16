#include "config_core.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_ALPHA_CHARS   'a':case 'A': \
                        case 'b':case 'B': \
                        case 'c':case 'C': \
                        case 'd':case 'D': \
                        case 'e':case 'E': \
                        case 'f':case 'F': \
                        case 'g':case 'G': \
                        case 'h':case 'H': \
                        case 'i':case 'I': \
                        case 'j':case 'J': \
                        case 'k':case 'K': \
                        case 'l':case 'L': \
                        case 'm':case 'M': \
                        case 'n':case 'N': \
                        case 'o':case 'O': \
                        case 'p':case 'P': \
                        case 'q':case 'Q': \
                        case 'r':case 'R': \
                        case 's':case 'S': \
                        case 't':case 'T': \
                        case 'u':case 'U': \
                        case 'v':case 'V': \
                        case 'w':case 'W': \
                        case 'x':case 'X': \
                        case 'y':case 'Y': \
                        case 'z':case 'Z'
#define CONFIG_NUM_CHARS '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9'
#define CONFIG_SPECIAL_CHARS '_':case '-'

#define CONFIG_EMIT_BUF_SIZE 256

#define SNPRINTF_SIZE_CONDITION(size) ((size) > 0 ? (size): 0)

typedef enum {
    CONFIG_BEGIN,
    CONFIG_SECTION,
    CONFIG_SECTION_NAME,
    CONFIG_SECTION_BODY,
    CONFIG_OPTION_KEY,
    CONFIG_OPTION_VALUE,
    CONFIG_PARSE_FAIL
} config_parser_state_t;


void config_option_construct(config_option_t * config_option) {
    if(config_option == NULL) {
        return;
    }

    config_option->next = NULL;
    strcpy(config_option->key, "");
    config_option->value = NULL;
    config_option->size = 0;
    config_option->type = '\0';
}

void config_option_destruct(config_option_t * config_option) {
    config_option->next = NULL;

    if(config_option->value != NULL) {
        free(config_option->value);
        config_option->value = NULL;
    }

    config_option->size = 0;
    config_option->type = '\0';
}

void config_option_set_value(config_option_t * config_option, const char * value) {
    int size = strlen(value);
    config_option->type = value[0];

    switch(config_option->type) {
        case CONFIG_OPTION_STR_TYPE:
            config_option->value = malloc(size);
            config_option->size = size;
            if(config_option->value == NULL) {
               return;
            } else {
                memcpy((char *)(config_option->value), value + 1, size);
            }
            break;

        case CONFIG_OPTION_NUM_INT_TYPE:
            config_option->value = (config_int_t *) malloc(sizeof(config_int_t));
            config_option->size = sizeof(config_int_t);
            if(config_option->value == NULL) {
                return;
            } else {
                sscanf(value + 1, "%d", (config_int_t *)(config_option->value));
            }
            break;

        case CONFIG_OPTION_NUM_UINT_TYPE:
            config_option->value = (config_uint_t *) malloc(sizeof(config_uint_t));
            config_option->size = sizeof(config_uint_t);
            if(config_option->value == NULL) {
                return;
            } else {
                sscanf(value + 1, "%u", (config_uint_t *)(config_option->value));
            }
            break;

        default:
            break;
    } // end switch
}


void config_section_construct(config_section_t * config_section, uint8_t id) {
    if(config_section == NULL) {
        return;
    }

    config_section->next = NULL;
    config_section->id = id;
    LIST_STRUCT_INIT(config_section, options);
}

void config_section_destruct(config_section_t * config_section) {
    config_option_t * it = NULL;
    config_option_t * next = NULL;

    //destruct and free all options of this section
    for(it = (config_option_t *) list_head(config_section->options); it != NULL; it = next) {
        next = (config_option_t *) list_item_next(it);
        config_option_destruct(it);
        free(it);
    }

    config_section->next = NULL;
    config_section->options_list = NULL;
    config_section->options = NULL;
}

config_option_t * config_section_create_option(config_section_t * config_section) {
    config_option_t * opt = (config_option_t *) malloc(sizeof(config_option_t));

    if(opt != NULL) {
        config_option_construct(opt);
        list_add(config_section->options, opt);
    }

    return opt;
}

config_option_t * config_section_find_option(config_section_t * config_section, const char * key) {
    config_option_t * opt;

    for (opt = (config_option_t *) list_head(config_section->options); opt != NULL; opt = (config_option_t *) list_item_next(
            (void *) opt)) {
        if (strcmp(opt->key, key) == 0)
            return opt;
    }
    return NULL;
}

config_int_t config_section_get_int(config_section_t * config_section, const char * key, config_int_t default_val) {
    config_int_t res;
    config_option_t * opt = config_section_find_option(config_section, key);

    if(opt->type != CONFIG_OPTION_NUM_INT_TYPE) {
        res = default_val;
    } else {
        res = *(config_int_t *)(opt->value);
    }

    return res;
}

config_uint_t config_section_get_uint(config_section_t * config_section, const char * key, config_uint_t default_val) {
    config_uint_t res;
    config_option_t * opt = config_section_find_option(config_section, key);

    if(opt->type != CONFIG_OPTION_NUM_UINT_TYPE) {
        res = default_val;
    } else {
        res = *(config_uint_t *)(opt->value);
    }

    return res;
}

//TODO: use strncpy
void config_section_get_str(config_section_t * config_section, const char * key, const char * default_val, char * dest, unsigned int dest_size) {
    config_option_t * opt = config_section_find_option(config_section, key);

    if(opt->type != CONFIG_OPTION_STR_TYPE) {
        strcpy(dest, default_val/*, dest_size*/);
    } else {
        strcpy(dest, (char *)(opt->value)/*, dest_size*/);
    }
}

int config_section_set_int(config_section_t * config_section, const char * key, config_int_t val) {
    config_option_t * opt = config_section_find_option(config_section, key);
    int res = 1;

    if  (opt == NULL) {
        opt = config_section_create_option(config_section);
        strncpy(opt->key, key, CONFIG_OPTION_KEY_LEN);
        opt->type = CONFIG_OPTION_NUM_INT_TYPE;
    }

    if(opt->type != CONFIG_OPTION_NUM_INT_TYPE) {
        res = -1;
    } else {
        opt->value = malloc(sizeof(config_int_t));
        *(config_int_t *)(opt->value) = val;
        opt->size = sizeof(config_int_t);
    }

    return res;
}

int config_section_set_uint(config_section_t * config_section, const char * key, config_uint_t val) {
    config_option_t * opt = config_section_find_option(config_section, key);
    int res = 1;

    if  (opt == NULL) {
        opt = config_section_create_option(config_section);
        strncpy(opt->key, key, CONFIG_OPTION_KEY_LEN);
        opt->type = CONFIG_OPTION_NUM_UINT_TYPE;
    }

    if(opt->type != CONFIG_OPTION_NUM_UINT_TYPE) {
        res = -1;
    } else {
        opt->value = malloc(sizeof(config_uint_t));
        *(config_uint_t *)(opt->value) = val;
        opt->size = sizeof(config_uint_t);
    }

    return res;
}

int config_section_set_str(config_section_t * config_section, const char * key, const char * val) {
    config_option_t * opt = config_section_find_option(config_section, key);
    int res = 1;

    if  (opt == NULL) {
        opt = config_section_create_option(config_section);
        strncpy(opt->key, key, CONFIG_OPTION_KEY_LEN);
        opt->type = CONFIG_OPTION_STR_TYPE;
    }

    if(opt->type != CONFIG_OPTION_STR_TYPE) {
        res = -1;
    } else {
        if(opt->value) free(opt->value);

        opt->size = strlen(val) + 1;
        opt->value = malloc(opt->size);

        strncpy(opt->value, val, opt->size);
    }

    return res;
}


void config_cnf_construct(config_cnf_t * config) {
    if(config == NULL) {
        return;
    }

    LIST_STRUCT_INIT(config, sections);
}

void config_cnf_destruct(config_cnf_t * config) {
    if(config == NULL) {
        return;
    }

    config_section_t * it = NULL;
    config_section_t * next = NULL;

    //destruct and free all sections of this config
    for(it = (config_section_t *) list_head(config->sections); it != NULL; it = next) {
        next = (config_section_t *) list_item_next(it);
        config_section_destruct(it);
        free(it);
    }

    config->sections_list = NULL;
    config->sections = NULL;
}

void config_cnf_copy(config_cnf_t * config, config_cnf_t * prototype) {
    if((config == NULL) || (prototype == NULL)) {
        return;
    }

    config->sections = &(config->sections_list);
    config->sections_list = prototype->sections_list;
}

int config_cnf_add_section(config_cnf_t * config, config_section_t * sect) {
    if((config == NULL) || (sect == NULL)) {
        return 2;
    }

    if(config_cnf_find_section(config, sect->id) == NULL) {
        list_add(config->sections, sect);
    } else {
        return 1;
    }

    return 0;
}

void config_cnf_del_section(config_cnf_t * config, config_section_t * sect) {
    if((config == NULL) || (sect == NULL)) {
        return;
    }

    list_remove(config->sections, (void *)sect);
}

config_section_t * config_cnf_find_section(config_cnf_t * config, uint8_t id) {
    config_section_t * sect = NULL;

    if(config != NULL) {
        for(sect = (config_section_t *) list_head(config->sections); sect != NULL; sect = (config_section_t *) list_item_next(sect)) {
            if(sect->id == id) break;
        }
    }

    return sect;
}

config_section_t * config_cnf_create_section(config_cnf_t * config, uint8_t id) {
    if(config == NULL) {
        return NULL;
    }

    config_section_t * sect = malloc(sizeof(config_section_t));
    if(sect != NULL) {
        config_section_construct(sect, id);
        if(config_cnf_add_section(config, sect) != 0) {
            free(sect);
            sect = NULL;
        }
    }

    return sect;
}


int config_parse(const char * src_str, config_cnf_t * dest_config) {
    char ch;
    config_section_t * cur_sect = NULL;
    config_option_t * cur_opt = NULL;
    config_option_t * it = NULL;
    const char * src_str_it;
    const char * src_str_end = src_str + strlen(src_str);
    unsigned int str_ind = 0;
    char str_buf[128];
    config_parser_state_t state = CONFIG_BEGIN;

    if((src_str == NULL) || (dest_config == NULL)) {
        return 2;
    }

    for(src_str_it = src_str; (src_str_it != src_str_end) && ((*src_str_it) != CONFIG_EOF); ++src_str_it) {
        ch = *src_str_it;

        switch(state) {
            case CONFIG_BEGIN:
                switch(ch) {
                    case '[':
                        state = CONFIG_SECTION;
                        str_ind = 0;
                        cur_sect = config_cnf_create_section(dest_config, 0);

                        if(cur_sect == NULL) state = CONFIG_PARSE_FAIL;
                        break;

                    case ' ':case '\n':
                        break;

                    default:
                        state = CONFIG_PARSE_FAIL;
                        break;
                } //end switch
                break;

            case CONFIG_SECTION:
                switch(ch) {
                    case CONFIG_NUM_CHARS:
                        state = CONFIG_SECTION_NAME;
                        str_buf[str_ind] = ch;
                        str_ind += 1;
                        break;

                    default:
                        state = CONFIG_PARSE_FAIL;
                        break;
                } //end switch
                break;

            case CONFIG_SECTION_NAME:
                switch(ch) {
                    case ']':
                        state = CONFIG_SECTION_BODY;
                        str_buf[str_ind] = '\0';
//                        sscanf(str_buf, "%u", &cur_sect->id);
                        unsigned int tmp;
                        sscanf(str_buf, "%u", &tmp);
                        cur_sect->id = (uint8_t)tmp;
                        str_ind = 0;
                        break;

                    case CONFIG_ALPHA_CHARS:
                    case CONFIG_NUM_CHARS:
                    case CONFIG_SPECIAL_CHARS:
                        str_buf[str_ind] = ch;
                        str_ind += 1;
                        break;

                    default:
                        state = CONFIG_PARSE_FAIL;
                        break;
                } //end switch
                break;

            case CONFIG_SECTION_BODY:
                switch(ch) {
                    case CONFIG_ALPHA_CHARS:
                        state = CONFIG_OPTION_KEY;
                        cur_opt = config_section_create_option(cur_sect);

                        if(cur_opt == NULL) {
                            state = CONFIG_PARSE_FAIL;
                        } else {
                            cur_opt->key[str_ind] = ch;
                            str_ind += 1;
                        }
                        break;

                    case '[':
                        state = CONFIG_SECTION;
                        str_ind = 0;
                        cur_sect = config_cnf_create_section(dest_config, 0);

                        if(cur_sect == NULL) state = CONFIG_PARSE_FAIL;
                        break;

                    case '\n': case ' ':
                        break;

                    default:
                        state = CONFIG_PARSE_FAIL;
                        break;
                } // end switch
                break;

            case CONFIG_OPTION_KEY:
                switch(ch) {
                    case CONFIG_ALPHA_CHARS:
                    case CONFIG_NUM_CHARS:
                    case CONFIG_SPECIAL_CHARS:
                        cur_opt->key[str_ind] = ch;
                        str_ind += 1;
                        break;

                    case '=':
                        state = CONFIG_OPTION_VALUE;
                        cur_opt->key[str_ind] = '\0';
                        str_ind = 0;

                        //if duplicate option found -- stop parsing
                        for(it = (config_option_t *) list_head(cur_sect->options); it != cur_opt; it = (config_option_t *) list_item_next(it)) {
                            if(strcmp(cur_opt->key, it->key) == 0) {
                                state = CONFIG_PARSE_FAIL;
                                break;
                            }
                        }
                        break;

                    default:
                        state = CONFIG_PARSE_FAIL;
                        break;
                } // end switch
                break;

            case CONFIG_OPTION_VALUE:
                switch(ch) {
                    case '\n':
                        state = CONFIG_SECTION_BODY;
                        str_buf[str_ind] = '\0';
                        config_option_set_value(cur_opt, str_buf);
                        str_ind = 0;
                        break;

                    default:
                        str_buf[str_ind] = ch;
                        str_ind += 1;
                        break;
                } //end switch
                break;

            case CONFIG_PARSE_FAIL:
                return 1;
                break;
        } //end switch
    } //end for

    return (state == CONFIG_PARSE_FAIL) ? 1 : 0;
}

char * config_emit(config_cnf_t * src_config) {
    config_section_t * sect_it;
    config_option_t * opt_it;
    char dest_str_buf[CONFIG_EMIT_BUF_SIZE];
    char * dest_str_buf_end = dest_str_buf + CONFIG_EMIT_BUF_SIZE;
    char * dest_str_buf_it = dest_str_buf;
    char * dest_str = NULL;

    if(src_config == NULL) {
        return NULL;
    }

    for(sect_it = (config_section_t *) list_head(src_config->sections); sect_it != NULL; sect_it = (config_section_t *) list_item_next(sect_it)) {
        //write section id
        dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "[%u]\n", sect_it->id);

        for(opt_it = (config_option_t *) list_head(sect_it->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
            dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "%s=%c", opt_it->key, opt_it->type);

            switch(opt_it->type) {
                case CONFIG_OPTION_STR_TYPE:
                    dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "%s", (char *)(opt_it->value));
                    break;

                case CONFIG_OPTION_NUM_INT_TYPE:
                    dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "%d", *(config_int_t *)(opt_it->value));
                    break;

                case CONFIG_OPTION_NUM_UINT_TYPE:
                    dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "%u", *(config_uint_t *)(opt_it->value));
                    break;

                default:
                    break;
            }

            dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "\n");
        }
    }
    dest_str_buf_it += snprintf(dest_str_buf_it, SNPRINTF_SIZE_CONDITION(dest_str_buf_end - dest_str_buf_it), "%c", CONFIG_EOF);

    // Check that all config information was recorded
    if(*(dest_str_buf_it - 1) == CONFIG_EOF) {
        dest_str = (char *)malloc(dest_str_buf_it - dest_str_buf + 1);

        if(dest_str != NULL) {
            memcpy((void *)dest_str, (void *)dest_str_buf, dest_str_buf_it - dest_str_buf);
            dest_str[dest_str_buf_it - dest_str_buf] = '\0';
        }
    }

    return dest_str;
}
