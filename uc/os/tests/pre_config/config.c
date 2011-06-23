#include "config.h"
#include "lib/malloc.h"

#include <stdio.h>
#include <stdlib.h>
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

int config_parse(FILE * file) {
    int ch;
    config_section_t * cur_sect = NULL;
    config_option_t * cur_opt = NULL;
    config_option_t * it = NULL;
    unsigned int str_ind = 0;
    char str_buf[128];
    config_parser_state_t state = CONFIG_BEGIN;

    ch = fgetc(file);
    while(ch != CONFIG_EOF) {
        switch(state) {
            case CONFIG_BEGIN:
                printf("%c : In CONFIG_BEGIN state.\n", ch);
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
                printf("%c : In CONFIG_SECTION state.\n", ch);
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
                printf("%c : In CONFIG_SECTION_NAME state.\n", ch);
                switch(ch) {
                    case ']':
                        state = CONFIG_SECTION_BODY;
                        cur_sect->name[str_ind] = '\0';
                        printf("Current section name: %s\n", cur_sect->name);
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
                printf("%c : In CONFIG_SECTION_BODY state.\n", ch);
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
                printf("%c : In CONFIG_OPTION_KEY state.\n", ch);
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

                        for(it = (config_option_t *) list_head(cur_sect->options); it != cur_opt; it = (config_option_t *) list_item_next(it)) {
                            printf("!!! Cheking %s and %s\n", cur_opt->key, it->key);
                            if(strcmp(cur_opt->key, it->key) == 0) {
                                printf("!!! -> FAIL\n");
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
                printf("%c : In CONFIG_OPTION_VALUE state.\n", ch);
                switch(ch) {
                    case '\n':
                        state = CONFIG_SECTION_BODY;
                        str_buf[str_ind] = '\0';
                        cur_opt->type = str_buf[0];

                        switch(cur_opt->type) {
                            case CONFIG_STR_TYPE:
                                cur_opt->value = malloc(str_ind);
                                cur_opt->size = str_ind;
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    memcpy((char *)(cur_opt->value), str_buf + 1, str_ind);
                                    printf("Readed: %s = %s\n", cur_opt->key, (char *)(cur_opt->value));
                                    printf("\tsize = %d\n", cur_opt->size);
                                }
                                break;

                            case CONFIG_NUM_INT_TYPE:
                                cur_opt->value = (config_int_t *) malloc(sizeof(config_int_t));
                                cur_opt->size = sizeof(config_int_t);
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    sscanf(str_buf + 1, "%d", (config_int_t *)(cur_opt->value));
                                    printf("Readed: %s = %d\n", cur_opt->key, *(config_int_t *)(cur_opt->value));
                                }
                                break;

                            case CONFIG_NUM_UINT_TYPE:
                                cur_opt->value = (config_uint_t *) malloc(sizeof(config_uint_t));
                                cur_opt->size = sizeof(config_uint_t);
                                if(cur_opt->value == NULL) {
                                    state = CONFIG_PARSE_FAIL;
                                } else {
                                    sscanf(str_buf + 1, "%u", (config_uint_t *)(cur_opt->value));
                                    printf("Readed: %s = %u\n", cur_opt->key, *(config_uint_t *)(cur_opt->value));
                                }
                                break;

                            default:
                                state = CONFIG_PARSE_FAIL;
                                break;
                        } // end switch
                        str_ind = 0;
                        break;

                    default:
                        str_buf[str_ind] = ch;
                        str_ind += 1;
                        break;
                } //end switch
                break;

            case CONFIG_PARSE_FAIL:
                printf("%c : In CONFIG_PARSE_FAIL state.\n", ch);
                return -1;
                break;
        } //end switch

        ch = fgetc(file);
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
 */
int config_open() {
    FILE * file;
    file = fopen("config.ini", "r");

    if(file == NULL) {
        printf("Could not open configuration file.\n");
        return -1;
    }
    printf("File opened.\n");

    config_close();
    config_cnf_construct(&config);

    if(config_parse(file) == -1) {
        printf("Could not parse configuration file.\n");
        fclose(file);
        return -1;
    }
    fclose(file);

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
 */
int config_save() {
    FILE * file;
    config_section_t * sect_it;
    config_option_t * opt_it;

    file = fopen("config.ini", "w");

    if(file == NULL) {
        printf("Could not open configuration file.\n");
        return -1;
    }
    printf("File opened.\n");

    for(sect_it = (config_section_t *) list_head(config.sections); sect_it != NULL; sect_it = (config_section_t *) list_item_next(sect_it)) {
        //write section name
        fputc('[', file);
        fwrite(sect_it->name, sizeof(char), strlen(sect_it->name), file);
        fputc(']', file);
        fputc('\n', file);

        for(opt_it = (config_option_t *) list_head(sect_it->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
            fwrite(opt_it->key, sizeof(char), strlen(opt_it->key), file);
            fputc('=', file);
            fputc(opt_it->type, file);

            switch(opt_it->type) {
                case CONFIG_STR_TYPE:
                    fwrite(opt_it->value, sizeof(char), opt_it->size - 1, file);
                    break;

                case CONFIG_NUM_INT_TYPE:
                    fprintf(file, "%d", *(config_int_t *)(opt_it->value));
                    break;

                case CONFIG_NUM_UINT_TYPE:
                    fprintf(file, "%u", *(config_uint_t *)(opt_it->value));
                    break;

                default:
                    break;
            }

            fputc('\n', file);
        }
    }
    fputc('~', file);
    fclose(file);
    printf("File closed.\n");

    return 1;
}

/*
 * Print configuration to stdout.
 */
void config_print() {
    config_section_t * sect_it = NULL;
    config_option_t * opt_it = NULL;

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
                    break;

                case CONFIG_NUM_INT_TYPE:
                    printf("%d", *(config_int_t *)(opt_it->value));
                    break;

                case CONFIG_NUM_UINT_TYPE:
                    printf("%d", *(config_uint_t *)(opt_it->value));
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

config_cnf_t * config_get() {
    return &config;
}
