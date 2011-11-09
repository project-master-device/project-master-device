/*
 * A library for working with modules.
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "lib/list.h"
#include "config.h"

//Я думаю это должно быть в конфиге.
enum {
    MODULE_SECTION1, MODULE_SECTION2, MODULE_LED, MODULE_NUMBER
};

typedef struct {
    config_section_t * config;
} module_t;

/*
 * Create new module by name (get pointer on section in config with that name) and add it to array.
 */
unsigned module_create(char *, int);

/*
 * Destroy module.
 */
void module_destroy(int);

/*
 * Initialize all modules (hardcode now).
 */
unsigned modules_init();

/*
 * Get module by it name in enum.
 */
module_t * module_get(int);


#endif /* MODULE_H_ */
