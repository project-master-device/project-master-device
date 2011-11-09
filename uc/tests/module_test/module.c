#include "module.h"
#include "lib/malloc.h"
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>

/*
 * Create module by name (get pointer on section in config with that name)
 */
unsigned create_module(char * name, int pos) {
	int i = 0;
	config_cnf_t * config;
	config_section_t * curr_sect, *tmp;

	module_t * module = (module_t *) malloc(sizeof(module_t)), *tmp1;
	module->config = NULL;
	if (module == NULL) return 0;

	config = config_get();
	curr_sect = (config_section_t *)list_head(config->sections);

	for(i = 0; i < MOD_NUMBER; ++i) {
		if (modules[i]) {
			if (strcmp(modules[i]->config->name, name) == 0) {
				curr_sect = modules[i]->config;
			}
		}
	}

	if (curr_sect == NULL) return 0; //какая-то херня случилась (конфиг пустой или у модуля с таким именем конфиг пустой)

	while (curr_sect != NULL) {
		if (strcmp(curr_sect->name, name) == 0) {
			module->config = curr_sect;
			break;
		}
		curr_sect = (config_section_t *)list_item_next(curr_sect);
	}
	if (module->config == NULL) return 0;

	modules[pos] = module;
	printf("Module %d -- \"%s\" initialized\n", pos, modules[pos]->config->name);
	return 1;
}

/*
 * Destroy module.
 */
void destroy_module(module_t * m) {
	printf("Module \"%s\" destroyed\n", m->config->name);
	free(m);
}

/*
 * Initialize all modules (hardcode, array or config?)
 */

unsigned init_modules() {
	return create_module("section", SECTION1) && create_module("section", SECTION2) && create_module("led", LED);//так, или из массива брать, или из конфига
}

module_t * get_module(int pos) {
	return modules[pos];
}

void print_modules() {
	int i;
	for(i = 0; i < MOD_NUMBER; ++i) {
		printf("%p \n", modules[i]);
		printf("\tname -- %s\n", modules[i]->config->name);
	}
}

