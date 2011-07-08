#include "led.h"

int pmd_net_led_write_data(bytearr_t* arr, const pmd_net_led_data_t * data) {
    if((data == NULL) || (arr.len == 0) || (arr.itself == NULL))
        return 1;

    switch(data->operation) {
    case PMD_LED_ON:
    case PMD_LED_OFF:
    case PMD_LED_TOGGLE:
        arr.itself[0] = data->operation;
        break;

    default:
        return 3;
        break;
    }

    return 0;
}

int pmd_net_led_read_data(const bytearr_t* arr, pmd_net_led_data_t * data) {
    if((data == NULL) || (arr.len == 0) || (arr.itself == NULL))
        return 1;

    switch(arr.itself[0]) {
    case PMD_LED_ON:
    case PMD_LED_OFF:
    case PMD_LED_TOGGLE:
        data->operation = arr.itself[0];
        break;

    default:
        return 3;
        break;
    }

    return 0;
}
