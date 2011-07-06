#ifndef PMD_LED_H_
#define PMD_LED_H_

#include "../../lib/bytearray.h"

#define PMD_NET_LED_ON      1
#define PMD_NET_LED_OFF     2
#define PMD_NET_LED_TOGGLE  3


typedef struct {
    uint8_t operation;
} pmd_net_led_data_t;

int pmd_net_led_write_data(bytearr_t* arr, const pmd_net_led_data_t * data);
int pmd_net_led_read_data(const bytearr_t* arr, pmd_net_led_data_t * data);

#endif /* PMD_LED_H_ */
