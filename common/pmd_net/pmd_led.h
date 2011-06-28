#ifndef PMD_LED_H_
#define PMD_LED_H_

#include "../net/can_net_middle.h"
#include <stdint.h>

#define PMD_NET_LED_ON      1
#define PMD_NET_LED_OFF     2
#define PMD_NET_LED_TOGGLE  3


typedef struct {
    uint8_t operation;
} pmd_led_data_t;

int pmd_led_write_data(const bytearr_t arr, pmd_led_data_t * data);
int pmd_led_read_data(const bytearr_t arr, pmd_led_data_t * data);

#endif /* PMD_LED_H_ */
