#include "system.h"

#include <string.h>

int pmd_net_system_config_write_data(bytearr_t arr, const pmd_net_system_config_data_t * data) {
    if(data == NULL)
        return 1;

    const char * config_str = NULL;

    switch(data->operation) {
    case PMD_NET_SYSTEM_CONFIG_REQUEST:
        arr.itself = (uint8_t *)malloc(sizeof(uint8_t));
        if(arr.itself == NULL) {
            return 2;
        }
        arr.len = sizeof(uint8_t);
        arr.itself[0] = data->operation;
        break;

    case PMD_NET_SYSTEM_CONFIG_FULL:
        if(data->config == NULL) {
            return 3;
        }

        config_str = config_emit(data->config);
        if(config_str == NULL) {
            return 4;
        }

        arr.len = strlen(config_str) + 1;
        arr.itself = (uint8_t *)malloc(sizeof(uint8_t) * arr.len);
        if(arr.itself == NULL) {
            arr.len = 0;
            return 2;
        }
        memcpy(arr.itself + 1, config_str, arr.len - 1);
        arr.itself[0] = data->operation;
        break;

    case PMD_NET_SYSTEM_CONFIG_SECTION_ADD:
        if(data->section == NULL) {
            return 5;
        }

        config_cnf_t tmp_cnf;
        config_cnf_construct(&tmp_cnf);
        list_add(tmp_cnf.sections, (void *)(data->section));

        config_str = config_emit(&tmp_cnf);
        if(config_str == NULL) {
            return 6;
        }

        arr.len = strlen(config_str) + 1;
        arr.itself = (uint8_t *)malloc(sizeof(uint8_t) * arr.len);
        if(arr.itself == NULL) {
            arr.len = 0;
            return 2;
        }
        memcpy(arr.itself + 1, config_str, arr.len - 1);
        arr.itself[0] = data->operation;
        break;

    case PMD_NET_SYSTEM_CONFIG_SECTION_DEL:
        arr.itself = (uint8_t *)malloc(sizeof(uint8_t) + 1);
        if(arr.itself == NULL) {
            return 2;
        }
        arr.len = sizeof(uint8_t) + 1;
        arr.itself[1] = data->section->id;
        arr.itself[0] = data->operation;
        break;
    }

    return 0;
}

int pmd_net_system_config_read_data(const bytearr_t arr, pmd_net_system_config_data_t * data) {
    if((data == NULL) || (arr.len == 0) || (arr.itself == NULL))
        return 1;

    int rc;
    char buf[arr.len];

    switch(arr.itself[0]) {
    case PMD_NET_SYSTEM_CONFIG_REQUEST:
        data->operation = arr.itself[0];
        break;

    case PMD_NET_SYSTEM_CONFIG_FULL:
        data->config = (config_cnf_t *)malloc(sizeof(config_cnf_t));
        if(data->config == NULL) {
            return 2;
        }
        config_cnf_construct(data->config);

        memcpy((void *)buf, arr.itself + 1, arr.len - 1);
        buf[arr.len - 1] = '\0';

        rc = config_parse(buf, data->config);
        if(rc != 0) {
            return 3;
        }
        data->operation = arr.itself[0];
        break;

    case PMD_NET_SYSTEM_CONFIG_SECTION_ADD:
        memcpy((void *)buf, arr.itself + 1, arr.len - 1);
        buf[arr.len - 1] = '\0';

        config_cnf_t tmp_cnf;
        config_cnf_construct(&tmp_cnf);
        rc = config_parse(buf, &tmp_cnf);
        if(rc != 0) {
            return 4;
        }
        data->section = (config_section_t *)list_head(tmp_cnf.sections);
        data->operation = arr.itself[0];
        break;

    case PMD_NET_SYSTEM_CONFIG_SECTION_DEL:
        if(arr.len < 2) {
            return 5;
        }

        data->section = (config_section_t *)malloc(sizeof(config_section_t));
        if(data->section == NULL) {
            return 2;
        }

        data->section->id = arr.itself[1];
        data->operation = arr.itself[0];
        break;
    }

    return 0;
}
