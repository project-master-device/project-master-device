#ifndef PMD_BUTTON_H_
#define PMD_BUTTON_H_

#include "../net/can_net_middle.h"
#include <stdint.h>

#define PMD_BUTTON_DOWN 1
#define PMD_BUTTON_UP   2

typedef struct {
    uint8_t operation;
} pmd_button_data_t;

int pmd_button_write_data(const bytearr_t arr, pmd_button_data_t * data);
int pmd_button_read_data(const bytearr_t arr, pmd_button_data_t * data);

#endif /* PMD_BUTTON_H_ */
