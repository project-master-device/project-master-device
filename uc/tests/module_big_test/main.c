#include <stdio.h>
//#include <string.h>
#include "config.h"
#include "module.h"
#include "module_manager.h"

enum decision {
    EXIT, PRINT, ADD_OPTION, REMOVE_OPTION, MODIFY_OPTION, SAVE, HELP
};

int main() {
    int decision, module;
    const char * help =
            "Help:\n\t0 - exit;\n\t1 - print modules;\n\t2 - add option;\n\t3 - remove option;\n\t4 - modify option\n\t5 - save config;\n\t6 - help\n";
    char  key[16], value[16];
    config_init();

    int st = config_open();
    if (st == 1) {
        printf("Parse success!\n");
        config_print();
    } else if (st == -1) {
        printf("Epic Fail :(\n");
    }

    if (init_modules())
        printf("Successful modules init!\n");
    else
        printf("Shit happened with module init!\n");

    print_modules();

    printf(help);
    scanf("%d", &decision);

    while (decision != EXIT) {
        switch (decision) {
        case PRINT:
            print_modules();
            break;
        case ADD_OPTION:
            printf("Input module number: ");
            scanf("%d", &module);

            printf("Input key: ");
            scanf("%s", key);

            printf("Input value: ");
            scanf("%s", value);

            module_add_option(get_module(module), key, value);
            printf("Option successfully added.\n");
            break;
        case REMOVE_OPTION:
            printf("Input module number: ");
            scanf("%d", &module);

            printf("Input key: ");
            scanf("%s", key);

            module_remove_option(get_module(module), key);
            printf("Option successfully removed.\n");
            break;
        case MODIFY_OPTION:
            printf("Input module number: ");
            scanf("%d", &module);

            printf("Input key: ");
            scanf("%s", key);

            printf("Input new value: ");
            scanf("%s", value);

            module_modify_option(get_module(module), key, value);
            printf("Option successfully modified.\n");
            break;
        case SAVE:
            config_save();
            break;
        case HELP:
            printf(help);
            break;
        default:
            break;
        }
        scanf("%d", &decision);
    }
    return 0;
}

