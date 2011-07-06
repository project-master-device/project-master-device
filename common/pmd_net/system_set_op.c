#include "system_set_op.h"

int pmd_net_system_set_op_write_data(bytearr_t * dest_arr, const pmd_net_system_set_op_data_t * source_data) {
    if((dest_arr == NULL) || (source_data == NULL))
        return 1;

    switch(source_data->operation) {
    case PMD_NET_SYSTEM_SET_OP_INIT:
    case PMD_NET_SYSTEM_SET_OP_NORMAL:
    case PMD_NET_SYSTEM_SET_OP_CONFIGURATION:
        dest_arr->len = 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        break;

    default:
        return 3;
        break;
    }

    return 0;
}

int pmd_net_system_set_op_read_data(const bytearr_t * source_arr, pmd_net_system_set_op_data_t * dest_data) {
    if((source_arr == NULL) || (dest_data == NULL))
        return 1;

    if((source_arr->itself == NULL) || (source_arr->len == 0))
        return 4;

    switch(source_arr->itself[0]) {
    case PMD_NET_SYSTEM_SET_OP_INIT:
    case PMD_NET_SYSTEM_SET_OP_NORMAL:
    case PMD_NET_SYSTEM_SET_OP_CONFIGURATION:
        dest_data->operation = source_arr->itself[0];
        break;

    default:
        return 3;
        break;
    }

    return 0;
}
