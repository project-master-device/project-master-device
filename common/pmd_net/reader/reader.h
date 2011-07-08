#ifndef PMD_READER_H_
#define PMD_READER_H_

#include <stdint.h>
#include "../../lib/bytearray.h"

#define PMD_NET_READER_MSG_LEN			3
//commands
#define PMD_NET_READER_SEND_MSG         0
#define PMD_NET_READER_GREEN_LED_ON     1
#define PMD_NET_READER_GREEN_LED_OFF    2
#define PMD_NET_READER_BEEP_ON          3
#define PMD_NET_READER_BEEP_OFF         4

typedef struct {
    uint8_t operation;
    uint8_t data[PMD_NET_READER_MSG_LEN];
} pmd_net_reader_data_t;


/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- unknown operation
 */
int pmd_net_reader_write_data(bytearr_t * dest_arr, const pmd_net_reader_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      3 -- unknown operation
 *      4 -- bad source data
 */
int pmd_net_reader_read_data(const bytearr_t * source_arr, pmd_net_reader_data_t * dest_data);

#endif /* PMD_READER_H_ */
