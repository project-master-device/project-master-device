#ifndef MODULE_H_
#define MODULE_H_

#include "lib/list.h"
#include "config.h"

#define MOD_NUMBER 3
enum {
	SECTION1,
	SECTION2,
	LED
};

typedef struct {
    config_section_t * config;
} module_t;

static module_t * modules[MOD_NUMBER];

unsigned create_module (char *, int);
unsigned init_modules();
void destroy_module(module_t *);
module_t * get_module(int);
void print_modules();

#endif /* MODULE_H_ */
