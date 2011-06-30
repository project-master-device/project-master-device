#include "pmd_button.h"

int pmd_button_write_data(const bytearr_t arr, pmd_button_data_t * data) {
    if((data == NULL) || (arr.len == 0) || (arr.itself == NULL))
        return 1;

    switch(data->operation) {
    case PMD_BUTTON_DOWN:
    case PMD_BUTTON_UP:
        arr.itself[0] = data->operation;
        break;

    default:
        return 3;
        break;
    }

    return 0;
}

int pmd_button_read_data(const bytearr_t arr, pmd_button_data_t * data) {
    if((data == NULL) || (arr.len == 0) || (arr.itself == NULL))
        return 1;

    switch(arr.itself[0]) {
    case PMD_BUTTON_DOWN:
    case PMD_BUTTON_UP:
        data->operation = arr.itself[0];
        break;

    default:
        return 3;
        break;
    }

    return 0;
}
