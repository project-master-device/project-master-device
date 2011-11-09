/*
 * A library for working with modules.
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "lib/list.h"
#include "config.h"

//Я думаю это должно быть в конфиге.
enum {
    SECTION1, SECTION2, LED, MOD_NUMBER
};

typedef struct {
    config_section_t * config;
} module_t;

/*
 * Create new module by name (get pointer on section in config with that name) and add it to array.
 */
unsigned create_module(char *, int);

/*
 * Destroy module.
 */
void destroy_module(int);

/*
 * Initialize all modules (hardcode now).
 */
unsigned init_modules();

/*
 * Get module by it name in enum.
 */
module_t * get_module(int);

void print_modules();

#endif /* MODULE_H_ */
