#include <stdio.h>
#include "config.h"

enum decision {
    EXIT,
    SAVE_CONFIG,
    ADD_SECTION
};

int main() {
    config_init();

    int st = config_open();
    if(st == 1) {
        printf("Parse success!\n");
        config_print();
    } else if (st == -1) {
        printf("Epic Fail :(\n");
    }

    const char * help = "Help:\n0 -- exit\n1 -- save config\n2 -- add section";
    config_section_t * sect = NULL;
    int decision;
    printf(help);
    printf("\nInput action: ");
    scanf("%d", &decision);

    while(decision != EXIT) {
        switch(decision) {
            case SAVE_CONFIG:
                config_save();
                break;

            case ADD_SECTION:
                printf("Input section name: ");
                sect = config_cnf_create_section(config_get());
                scanf("%s", sect->name);
                break;

            default:
                printf("What? no no David Blaine! no!\n");
                break;
        }

        printf("\nInput action: ");
        scanf("%d", &decision);
    }

    return 0;
}

