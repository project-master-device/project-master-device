#ifndef PMD_BUTTON_H_
#define PMD_BUTTON_H_

#include <stdint.h>
#include "../../lib/bytearray.h"

#define PMD_NET_BUTTON_DOWN 1
#define PMD_NET_BUTTON_UP   2

typedef struct {
    uint8_t operation;
} pmd_net_button_data_t;

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- unknown operation
 */
int pmd_net_button_write_data(bytearr_t * dest_arr, const pmd_net_button_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      3 -- unknown operation
 *      4 -- bad source data
 */
int pmd_net_button_read_data(const bytearr_t * source_arr, pmd_net_button_data_t * dest_data);

#endif /* PMD_BUTTON_H_ */
