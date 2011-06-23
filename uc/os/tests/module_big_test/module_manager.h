/*
 * A tool for managing module's options.
 */

#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_

#include "module.h"

/*
 * Add new option to module.
 */
void module_add_option(module_t *, const char *, const char *); 	//module_add_option(get_module(LED), "level", "sHIGH")

/*
 * Remove option from module.
 */
void module_remove_option(module_t *, const char *); 		    	//module_remove_option(get_module(LED), "level")

/*
 * Change value of module's option.
 */
void module_modify_option(module_t *, const char *, const char *);  //module_modify_option(get_module(LED), "portb", "i321")

#endif /* MODULE_MANAGER_H_ */
