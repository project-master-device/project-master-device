#include "module.h"
#include "lib/malloc.h"
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>

/*
 * An array that holds all modules.
 */
static module_t * modules[MOD_NUMBER];

unsigned create_module(char * name, int pos) {
	int i = 0;
	config_cnf_t * config;
	config_section_t * curr_sect = NULL, *tmp;

	module_t * module = (module_t *) malloc(sizeof(module_t)), *tmp1;
	module->config = NULL;
	if (module == NULL) return 0;

	config = config_get();

	for(i = 0; i < MOD_NUMBER; ++i) {
		if (modules[i]) {
			if (strcmp(modules[i]->config->name, name) == 0) {
				curr_sect = modules[i]->config;
			}
		}
	}

	if (curr_sect == NULL)  curr_sect = (config_section_t *)list_head(config->sections); //если раньше не было
	else  curr_sect = (config_section_t *)list_item_next(curr_sect); //если было, идем дальше

	while (curr_sect != NULL) {
		if (strcmp(curr_sect->name, name) == 0) {
			module->config = curr_sect;
			break;
		}
		curr_sect = (config_section_t *)list_item_next(curr_sect);
	}
	if (module->config == NULL) return 0;

	modules[pos] = module;
	return 1;
}

void destroy_module(int pos) {
    printf("Module %s destroyed.\n", modules[pos]->config->name);
    free(get_module(pos));
}

unsigned init_modules() {
	return create_module("section", SECTION1) && create_module("section", SECTION2) && create_module("led", LED);//так, или из массива брать, или из конфига
}

module_t * get_module(int pos) {
	return modules[pos];
}

void print_modules() {
	int i;
	config_option_t * opt_it;
	for(i = 0; i < MOD_NUMBER; ++i) {
		printf("%d: \n", i);
		printf("\tname -- %s\n", modules[i]->config->name);
		printf("\toptions:\n");
		for(opt_it = (config_option_t *) list_head(modules[i]->config->options); opt_it != NULL; opt_it = (config_option_t *) list_item_next(opt_it)) {
		            printf("\t\t%s=", opt_it->key);

		            switch(opt_it->type) {
		                case 's':
		                    printf("%s\n", (char *)opt_it->value);
		                    break;

		                case 'i':
		                    printf("%d\n", *(config_int_t *)(opt_it->value));
		                    break;

		                case 'I':
		                    printf("%d\n", *(config_uint_t *)(opt_it->value));
		                    break;

		                default:
		                    printf("<NOT CORRECT VALUE>");
		                    break;
		            }
		}
		printf("\n\n");
	}
}

