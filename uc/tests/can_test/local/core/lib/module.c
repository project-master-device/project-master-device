#include "module.h"
#include "lib/malloc.h"
#include <string.h>

/*
 * An array that holds all modules.
 */
static module_t * modules[MODULE_NUMBER];

unsigned module_create(char * name, int pos) {
    int i = 0;
    config_cnf_t * config;
    config_section_t * curr_sect = NULL;

    module_t * module = (module_t *) malloc(sizeof(module_t));
    module->config = NULL;
    if (module == NULL)
        return 0;

    config = config_get();

    for (i = 0; i < MODULE_NUMBER; ++i) {
        if (modules[i]) {
            if (strcmp(modules[i]->config->name, name) == 0) {
                curr_sect = modules[i]->config;
            }
        }
    }

    if (curr_sect == NULL)
        curr_sect = (config_section_t *) list_head(config->sections); //если раньше не было
    else
        curr_sect = (config_section_t *) list_item_next(curr_sect); //если было, идем дальше

    while (curr_sect != NULL) {
        if (strcmp(curr_sect->name, name) == 0) {
            module->config = curr_sect;
            break;
        }
        curr_sect = (config_section_t *) list_item_next(curr_sect);
    }
    if (module->config == NULL)
        return 0;

    modules[pos] = module;
    return 1;
}

void module_destroy(int pos) {
    free(module_get(pos));
}
//XXX
unsigned modules_init() {
    return module_create("section", MODULE_SECTION1) && module_create("section", MODULE_SECTION2)
            && module_create("led", MODULE_LED);//так, или из массива брать, или из конфига
}

module_t * module_get(int pos) {
    return modules[pos];
}

