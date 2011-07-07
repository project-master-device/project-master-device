#ifndef SYSTEM_SET_OP_H_
#define SYSTEM_SET_OP_H_

#include "../net/can_net_middle.h"

#include <stdint.h>

typedef struct {
    uint8_t operation;
} pmd_net_system_set_op_data_t;


#define PMD_NET_SYSTEM_SET_OP_INIT              1
#define PMD_NET_SYSTEM_SET_OP_NORMAL            2
#define PMD_NET_SYSTEM_SET_OP_CONFIGURATION     3

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- unknown operation
 */
int pmd_net_system_set_op_write_data(bytearr_t * dest_arr, const pmd_net_system_set_op_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      3 -- unknown operation
 *      4 -- bad source data
 */
int pmd_net_system_set_op_read_data(const bytearr_t * source_arr, pmd_net_system_set_op_data_t * dest_data);

#endif /* SYSTEM_SET_OP_H_ */
