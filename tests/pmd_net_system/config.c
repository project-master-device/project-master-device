#include <stdio.h>
#include "../../common/pmd_net/system.h"


void create_config(config_cnf_t * cnf) {
    if(cnf != NULL) {
        //create led 1
        config_section_t * sect = config_cnf_create_section(cnf);
        sect->id = 2;
        config_section_set_str(sect, "type", "led");
        config_section_set_str(sect, "test", "trololo");
        config_section_set_uint(sect, "ddr", 36);
        config_section_set_uint(sect, "port", 37);
        config_section_set_uint(sect, "offset", 4);
    }
}

void write_config(bytearr_t * arr) {
    if(arr) {
        arr->len = 0;
        arr->itself = NULL;
        pmd_net_system_config_data_t cd;

        config_cnf_t cnf;
        config_cnf_construct(&cnf);
        create_config(&cnf);

        cd.operation = PMD_NET_SYSTEM_CONFIG_FULL;
        cd.config = &cnf;
        cd.section = NULL;

        pmd_net_system_config_write_data(arr, &cd);
    }
}

void read_config(bytearr_t * arr) {
    if(arr) {
        pmd_net_system_config_data_t cd;
        cd.config = NULL;
        cd.section = NULL;
        pmd_net_system_config_read_data(arr, &cd);

        if(cd.config) {
            config_cnf_t cnf = *(cd.config);
            cnf.sections = &cnf.sections_list;
            free(cd.config);
            if(cnf.sections_list == *(cnf.sections))
                printf("GOOD\n");
            else
                printf("BAD\n");
        }
    }
}

int main() {
    bytearr_t arr;

    write_config(&arr);
    read_config(&arr);

    return 0;
}
