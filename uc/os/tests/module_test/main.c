#include <stdio.h>
#include "config.h"
#include "module.h"

int main() {
    config_init();

    int st = config_open();
    if(st == 1) {
        printf("Parse success!\n");
        config_print();
    } else if (st == -1) {
        printf("Epic Fail :(\n");
    }

    if (init_modules()) printf("Successful modules init!\n");
    else printf("Shit happened with module init!\n");

    print_modules();

    destroy_module(get_module(SECTION1));
    destroy_module(get_module(SECTION2));
    destroy_module(get_module(LED));

    return 0;
}

