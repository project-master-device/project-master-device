#include "reader.h"
#include <stddef.h>
#include <stdlib.h>

int pmd_net_reader_write_data(bytearr_t * dest_arr, const pmd_net_reader_data_t * source_data) {
    if((dest_arr == NULL) || (source_data == NULL))
        return 1;

    uint8_t i;

    switch (source_data->operation) {
    case PMD_NET_READER_GREEN_LED_ON:
    case PMD_NET_READER_GREEN_LED_OFF:
    case PMD_NET_READER_BEEP_ON:
    case PMD_NET_READER_BEEP_OFF:
        dest_arr->len = 1;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        break;

    case PMD_NET_READER_SEND_MSG:
        dest_arr->len = 1 + PMD_NET_READER_MSG_LEN;
        dest_arr->itself = (uint8_t *)malloc(sizeof(uint8_t) * dest_arr->len);
        if(dest_arr->itself == NULL) {
            dest_arr->len = 0;
            return 2;
        }
        dest_arr->itself[0] = source_data->operation;
        for (i = 0; i < PMD_NET_READER_MSG_LEN; i++) {
            dest_arr->itself[i + 1] = source_data->data[i];
        }
        break;

    default:
        return 3;
    }
    return 0;
}

int pmd_net_reader_read_data(const bytearr_t * source_arr, pmd_net_reader_data_t * dest_data) {
    if((source_arr == NULL) || (dest_data == NULL))
        return 1;

    if((source_arr->itself == NULL) || (source_arr->len == 0))
        return 4;

    uint8_t i;

    switch (source_arr->itself[0]) {
    case PMD_NET_READER_GREEN_LED_ON:
    case PMD_NET_READER_GREEN_LED_OFF:
    case PMD_NET_READER_BEEP_ON:
    case PMD_NET_READER_BEEP_OFF:
        dest_data->operation = source_arr->itself[0];
        break;

    case PMD_NET_READER_SEND_MSG:
        if(source_arr->len < (1 + PMD_NET_READER_MSG_LEN))
            return 4;

        dest_data->operation = source_arr->itself[0];
        for (i = 0; i < PMD_NET_READER_MSG_LEN; i++) {
            dest_data->data[i] = source_arr->itself[i + 1];
        }
        break;

    default:
        return 3;
    }
    return 0;
}
