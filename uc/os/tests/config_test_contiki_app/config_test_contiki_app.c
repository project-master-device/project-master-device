#include "contiki.h"
#include "lib/config.h"

#include <stdio.h>

PROCESS(config_test_contiki_app, "Contiki OS config module test");
AUTOSTART_PROCESSES(&config_test_contiki_app);

PROCESS_THREAD(config_test_contiki_app, ev, data) {
    PROCESS_BEGIN();
    printf("Hello, World!!\n\n");

    config_init();
    int st = config_open();
    if(st == 1) {
        printf("Parse success!\n");
        config_print();
        printf("\n");

        config_cnf_t * cnf = config_get();
        config_section_t * sect = list_head(cnf->sections);

        printf("Set first section:\n");
        printf("\topt = Ololo [string]\n");
        printf("\topt1 = 100500 [int]\n");

        if(config_section_set_str(sect, "opt", "Ololo") == -1) {
            printf("Set opt failed\n");
        }
        if(config_section_set_int(sect, "opt1", 100500) == -1) {
            printf("Set opt1 failed\n");
        }

        config_print();

        printf("Writing config\n");
        if(config_save() == 1) {
            printf("Successfully written\n");
        } else {
            printf("Write failed\n");
        }

    } else {
        printf("Epic Fail :(\n");
    }

    printf("\nGoodbye, World!!\n");
    PROCESS_END();
}
