#ifndef PMD_NET_BUTTON_H_
#define PMD_NET_BUTTON_H_

#include "../../lib/bytearray.h"

#define PMD_NET_BUTTON_DOWN 1
#define PMD_NET_BUTTON_UP   2

typedef struct {
    uint8_t operation;
} pmd_net_button_data_t;

int pmd_net_button_write_data(bytearr_t* arr, const pmd_net_button_data_t * data);
int pmd_net_button_read_data(const bytearr_t* arr, pmd_net_button_data_t * data);

#endif /* PMD_NET_BUTTON_H_ */
