#ifndef CONFIG_H_
#define CONFIG_H_

#include "lib/list.h"
#include "contiki-conf.h"

typedef int                     config_int_t;
typedef unsigned int            config_uint_t;

typedef struct {
    void * next;

    char key[CONFIG_OPTION_KEY_LEN];
    void * value;
    unsigned int size;
    char type;
} config_option_t;

void config_option_construct(config_option_t *);
void config_option_destruct(config_option_t *);


typedef struct {
    void * next;

    char name[CONFIG_SECTION_NAME_LEN];
    LIST_STRUCT(options);
} config_section_t;

void config_section_construct(config_section_t *);
void config_section_destruct(config_section_t *);
config_option_t * config_section_create_option(config_section_t *);


typedef struct  {
    LIST_STRUCT(sections);
} config_cnf_t;

void config_cnf_construct(config_cnf_t *);
void config_cnf_destruct(config_cnf_t *);
config_section_t * config_cnf_create_section(config_cnf_t *);


void config_init();
int config_open();
void config_close();
int config_save();
void config_print();
config_cnf_t * config_get();

#endif /* CONFIG_H_ */
