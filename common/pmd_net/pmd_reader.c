#include "pmd_reader.h"
#include <stddef.h>

int pmd_reader_write_data(const bytearr_t arr, pmd_reader_data_t * data) {
    if ((arr.len == 0) || (arr.itself == NULL) || (data == NULL))
        return 1;
    uint8_t i;

    switch (data->operation) {
    case PMD_READER_GREEN_LED_ON:
    case PMD_READER_GREEN_LED_OFF:
    case PMD_READER_BEEP_ON:
    case PMD_READER_BEEP_OFF:
        arr.itself[0] = data->operation;
        break;
    case PMD_READER_SEND_MSG:
        if (arr.len < PMD_READER_MSG_LEN + 1)
            return 2;
        arr.itself[0] = data->operation;
        for (i = 0; i < PMD_READER_MSG_LEN; i++) {
            arr.itself[i + 1] = data->data[i];
        }
        break;
    default:
        return 3;
    }
    return 0;
}

int pmd_reader_read_data(const bytearr_t arr, pmd_reader_data_t * data) {
    if ((arr.len == 0) || (arr.itself == NULL) || (data == NULL))
        return 1;
    uint8_t i;

    switch (arr.itself[0]) {
    case PMD_READER_GREEN_LED_ON:
    case PMD_READER_GREEN_LED_OFF:
    case PMD_READER_BEEP_ON:
    case PMD_READER_BEEP_OFF:
        data->operation = arr.itself[0];
        break;
    case PMD_READER_SEND_MSG:
        if (arr.len < PMD_READER_MSG_LEN + 1)
            return 2;
        data->operation = arr.itself[0];
        for (i = 0; i < PMD_READER_MSG_LEN; i++) {
            data->data[i] = arr.itself[i + 1];
        }
        break;
    default:
        return 3;
    }
    return 0;
}
