#include "module_manager.h"
#include "config.h"
#include "lib/list.h"
#include <string.h>


void module_add_option(module_t * module, const char * key, const char * value) {
    config_option_t * option;
    char s;
    if ((module == NULL) || (module->config == NULL))
        return;

    option = config_section_create_option(module->config);
    if (option == NULL) return;
    strcpy(option->key,key);
    config_option_set_value(option, value);
}

void module_remove_option(module_t * module, const char * key) {
    config_option_t * option;

    if ((module == NULL) || (module->config == NULL))
        return;

    option = config_section_find_option(module->config, key);
    if (option != NULL)
        list_remove(module->config->options, option);
}

void module_modify_option(module_t * module, const char * key, const char * value) {
    config_option_t * option;

    if ((module == NULL) || (module->config == NULL))
        return;

    option = config_section_find_option(module->config, key);
    if (option == NULL) return;
    config_option_set_value(option, value);
}
