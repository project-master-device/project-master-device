#ifndef PMD_NET_SYS_SET_OP_H_
#define PMD_NET_SYS_SET_OP_H_

#include "../../lib/bytearray.h"

typedef struct {
    uint8_t operation;
    // ...
} pmd_net_sys_set_op_data_t;


// set operation mode message operations
//#define PMD_NET_SYS_...				1


int pmd_net_sys_set_op_write_data(bytearr_t * dest_arr, const pmd_net_sys_set_op_data_t * source_data);

int pmd_net_sys_set_op_read_data(const bytearr_t * source_arr, pmd_net_sys_set_op_data_t * dest_data);

#endif /* PMD_NET_SYS_SET_OP_H_ */
