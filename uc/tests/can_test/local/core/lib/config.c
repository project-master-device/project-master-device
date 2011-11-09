#include "config.h"
#include "cfs/cfs.h"
#include "malloc.h"

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

#define CONFIG_EOF '~'

#define CONFIG_STR_TYPE          's'
#define CONFIG_NUM_INT_TYPE      'i'
#define CONFIG_NUM_UINT_TYPE     'I'

#define CONFIG_FILENAME "conf.ini"

typedef enum {
    CONFIG_BEGIN,
    CONFIG_SECTION,
    CONFIG_SECTION_NAME,
    CONFIG_SECTION_BODY,
    CONFIG_OPTION_KEY,
    CONFIG_OPTION_VALUE,
    CONFIG_PARSE_FAIL
} config_parser_state_t;


static config_cnf_t config;


void config_option_construct(config_option_t * config_option) {
    config_option->next = NULL;
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
        case CONFIG_STR_TYPE:
            config_option->value = malloc(size);
            config_option->size = size;
            if(config_option->value == NULL) {
               return;
            } else {
                memcpy((char *)(config_option->value), value + 1, size);
            }
            break;

        case CONFIG_NUM_INT_TYPE:
            config_option->value = (config_int_t *) malloc(sizeof(config_int_t));
            config_option->size = sizeof(config_int_t);
            if(config_option->value == NULL) {
                return;
            } else {
                sscanf(value + 1, "%d", (config_int_t *)(config_option->value));
            }
            break;

        case CONFIG_NUM_UINT_TYPE:
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

void config_section_construct(config_section_t * config_section) {
    config_section->next = NULL;
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

config_option_t * config_section_create_option(config_section_t * section) {
    config_option_t * opt = (config_option_t *) malloc(sizeof(config_option_t));

    if(opt != NULL) {
        config_option_construct(opt);
        list_add(section->options, opt);
    }

    return opt;
}

config_option_t * config_section_find_option(config_section_t * sect, const char * key) {
    config_option_t * opt;

    for (opt = (config_option_t *) list_head(sect->options); opt != NULL; opt = (config_option_t *) list_item_next(
            (void *) opt)) {
        if (strcmp(opt->key, key) == 0)
            return opt;
    }
    return NULL;
}



void config_cnf_construct(config_cnf_t * config) {
    LIST_STRUCT_INIT(config, sections);
}

void config_cnf_destruct(config_cnf_t * config) {
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

config_section_t * config_cnf_create_section(config_cnf_t * config) {
    config_section_t * sect = malloc(sizeof(config_section_t));

    if(sect != NULL) {
        config_section_construct(sect);
        list_add(config->sections, sect);
    }

    return sect;
}

/*
 * \param sect          configuration section structure pointer
 * \param key           option key in given section
 * \param defalut_val   default return value on failure
 *
 * \return              option value
 */
config_int_t config_section_get_int(config_section_t * sect, const char * key, config_int_t default_val) {
    config_int_t res;
    config_option_t * opt = config_section_find_option(sect, key);

    if(opt->type != CONFIG_NUM_INT_TYPE) {
        res = default_val;
    } else {
        res = *(config_int_t *)(opt->value);
    }

    return res;
}

config_uint_t config_section_get_uint(config_section_t * sect, const char * key, config_uint_t default_val) {
    config_uint_t res;
    config_option_t * opt = config_section_find_option(sect, key);

    if(opt->type != CONFIG_NUM_UINT_TYPE) {
        res = default_val;
    } else {
        res = *(config_uint_t *)(opt->value);
    }

    return res;
}

/*
 * \param sect          configuration section structure pointer
 * \param key           option key in given section
 * \param defalut_val   default return value on failure
 * \param dest          destination value buffer
 * \param dest_size     destination buffer size
 */
void config_section_get_str(config_section_t * sect, const char * key, const char * default_val, char * dest, unsigned int dest_size) {
    config_option_t * opt = config_section_find_option(sect, key);

    if(opt->type != CONFIG_STR_TYPE) {
        strncpy(dest, default_val, dest_size);
    } else {
        strncpy(dest, (char *)(opt->value), dest_size);
    }
}

/*
 * \param sect  configuration section structure pointer
 * \param key   option key in given section
 * \param val   set value
 *
 * \return      1 if successful, -1 on failure
 */
int config_section_set_int(config_section_t * sect, const char * key, config_int_t val) {
    config_option_t * opt = config_section_find_option(sect, key);
    int res = 1;

    if(opt->type != CONFIG_NUM_INT_TYPE) {
        res = -1;
    } else {
        *(config_int_t *)(opt->value) = val;
    }

    return res;
}

int config_section_set_uint(config_section_t * sect, const char * key, config_uint_t val) {
    config_option_t * opt = config_section_find_option(sect, key);
    int res = 1;

    if(opt->type != CONFIG_NUM_UINT_TYPE) {
        res = -1;
    } else {
        *(config_uint_t *)(opt->value) = val;
    }

    return res;
}

int config_section_set_str(config_section_t * sect, const char * key, const char * val) {
    config_option_t * opt = config_section_find_option(sect, key);
    int res = 1;

    if(opt->type != CONFIG_STR_TYPE) {
        res = -1;
    } else {
        if(opt->value) free(opt->value);

        opt->size = strlen(val) + 1;
        opt->value = malloc(opt->size);

        strncpy(opt->value, val, opt->size);
    }

    return res;
}

int config_parse(int fd) {
    char ch;
    config_section_t * cur_sect = NULL;
    config_option_t * cur_opt = NULL;
    config_option_t * it = NULL;
    unsigned int str_ind = 0;
    char str_buf[128];
    config_parser_state_t state = CONFIG_BEGIN;

    //read 1 byte from eeprom
    cfs_read(fd, &ch, 1);
    while(ch != CONFIG_EOF) {
        switch(state) {
            case CONFIG_BEGIN:
                switch(ch) {
                    case '[':
                        state = CONFIG_SECTION;
                        str_ind = 0;
                        cur_sect = config_cnf_create_section(&config);

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
                    case CONFIG_ALPHA_CHARS:
                        state = CONFIG_SECTION_NAME;
                        cur_sect->name[str_ind] = ch;
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
                        cur_sect->name[str_ind] = '\0';
                        str_ind = 0;
                        break;

                    case CONFIG_ALPHA_CHARS: 
                    case CONFIG_NUM_CHARS:
                    case CONFIG_SPECIAL_CHARS:
                        cur_sect->name[str_ind] = ch;
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
                        cur_sect = config_cnf_create_section(&config);

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
                        config_option_set_value(cur_opt, str_buf); //пока закомментил
                        /*cur_opt->type = str_buf[0];

                        switch(cur_opt->type) {
                            case CONFIG_STR_TYPE:
                                cur_opt->value = malloc(str_ind);
                                cur_opt->size = str_ind;
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    memcpy((char *)(cur_opt->value), str_buf + 1, str_ind);
                                }
                                break;

                            case CONFIG_NUM_INT_TYPE:
                                cur_opt->value = (config_int_t *) malloc(sizeof(config_int_t));
                                cur_opt->size = sizeof(config_int_t);
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    sscanf(str_buf + 1, "%d", (config_int_t *)(cur_opt->value));
                                }
                                break;

                            case CONFIG_NUM_UINT_TYPE:
                                cur_opt->value = (config_uint_t *) malloc(sizeof(config_uint_t));
                                cur_opt->size = sizeof(config_uint_t);
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    sscanf(str_buf + 1, "%u", (config_uint_t *)(cur_opt->value));
                                }
                                break;

                            default:
                                state = CONFIG_PARSE_FAIL;
                                break;
                        } // end switch */
                        str_ind = 0;
                        break;

                    default:
                        str_buf[str_ind] = ch;
                        str_ind += 1;
                        break;
                } //end switch
                break;

            case CONFIG_PARSE_FAIL:
                return -1;
                break;
        } //end switch

        //read next 1 byte from eeprom
        cfs_read(fd, &ch, 1);
    } //end while

    return (state == CONFIG_PARSE_FAIL) ? -1 : 1;
}

/*
 * Initialize config_t structure.
 * Should be called before using config.
 */
void config_init() {
    config_cnf_construct(&config);
}

/*
 * Load configuration to memory.
 * Returns 1 if successful and -1 on failure.
 */
int config_open() {
    int filemode = CFS_READ;
    int fd = cfs_open(CONFIG_FILENAME, filemode);

    if(fd == -1) {
        cfs_close(fd);
        fd = cfs_open(CONFIG_FILENAME, filemode);
        if(fd == -1) {
            return -1;
        }
    }

    config_close();
    config_cnf_construct(&config);

    if(config_parse(fd) == -1) {
        cfs_close(fd);
        return -1;
    }
    cfs_close(fd);

    return 1;
}

/*
 * Clean configuration from memory.
 */
void config_close() {
    config_cnf_destruct(&config);
}

/*
 * Save configuration from memory to the file.
 * Returns 1 if successful and -1 on failure.
 */
int config_save() {
    int filemode = CFS_WRITE;
    config_section_t * sect_it;
    config_option_t * opt_it;
    char str_buf[8];
    int fd = cfs_open(CONFIG_FILENAME, filemode);

    if(fd == -1) {
        cfs_close(fd);
        fd = cfs_open(CONFIG_FILENAME, filemode);
        if(fd == -1) {
            return -1;
        }
    }

    for(sect_it = (config_section_t *) list_head(config.sections); sect_it != NULL; sect_it = (config_section_t *) list_item_next(sect_it)) {
        //write section name
        cfs_write(fd, (void *)"[", 1);
        cfs_write(fd, (void *)(sect_it->name), strlen(sect_it->name));
        cfs_write(fd, (void *)"]\n", 2);

        for(opt_it = (config_option_t *) list_head(sect_it->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
            cfs_write(fd, (void *)(opt_it->key), strlen(opt_it->key));
            cfs_write(fd, (void *)"=", 1);
            cfs_write(fd, (void *)(&(opt_it->type)), 1);

            switch(opt_it->type) {
                case CONFIG_STR_TYPE:
                    cfs_write(fd, (void *)(opt_it->value), opt_it->size - 1);
                    break;

                case CONFIG_NUM_INT_TYPE:
                    sprintf(str_buf, "%d", *(config_int_t *)(opt_it->value));
                    cfs_write(fd, (void *)str_buf, strlen(str_buf));
                    break;

                case CONFIG_NUM_UINT_TYPE:
                    sprintf(str_buf, "%u", *(config_uint_t *)(opt_it->value));
                    cfs_write(fd, (void *)str_buf, strlen(str_buf));
                    break;

                default:
                    break;
            }

            cfs_write(fd, (void *)"\n", 1);
        }
    }
    cfs_write(fd, (void *)"~", 1);
    cfs_close(fd);

    return 1;
}

config_cnf_t * config_get() {
    return &config;
}

#ifndef NDEBUG
/*
 * Print configuration to stdout.
 */
void config_print() {
    config_section_t * sect_it = NULL;
    config_option_t * opt_it = NULL;
    char buf[255];

    for(sect_it = (config_section_t *) list_head(config.sections); sect_it != NULL; sect_it = (config_section_t *) list_item_next(sect_it)) {
        printf("\n-- section: %s (at address %p) --\n", sect_it->name, sect_it);
        printf("next address: %p\n", sect_it->next);
        printf("options:\n");
        for(opt_it = (config_option_t *) list_head(sect_it->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
            printf("%p\n", opt_it);
            printf("\t%s=", opt_it->key);

            switch(opt_it->type) {
                case CONFIG_STR_TYPE:
                    printf("%s", (char *)opt_it->value);
                    config_section_get_str(sect_it, opt_it->key, "<default val>", buf, 255);
                    printf("\n\tusing access function: %s", buf);
                    break;

                case CONFIG_NUM_INT_TYPE:
                    printf("%d", *(config_int_t *)(opt_it->value));
                    printf("\n\tusing access function: %d", config_section_get_int(sect_it, opt_it->key, -100500));
                    break;

                case CONFIG_NUM_UINT_TYPE:
                    printf("%u", *(config_uint_t *)(opt_it->value));
                    printf("\n\tusing access function: %u", config_section_get_uint(sect_it, opt_it->key, 100500));
                    break;

                default:
                    printf("<NOT CORRECT VALUE>");
                    break;
            }

            printf("\n\tsize = %d\n", opt_it->size);
            printf("\tnext address: %p\n", opt_it->next);
        }
    }
}
#endif

