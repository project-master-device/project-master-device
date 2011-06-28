#ifndef PMD_READER_H_
#define PMD_READER_H_

#include <stdint.h>
#include "../net/can_net_middle.h"

#define PMD_READER_MSG_LEN 3
//commands
#define PMD_READER_SEND_MSG         0
#define PMD_READER_GREEN_LED_ON     1
#define PMD_READER_GREEN_LED_OFF    2

typedef struct {
    uint8_t operation;
    uint8_t data[PMD_READER_MSG_LEN];
} pmd_reader_data_t;

int pmd_reader_write_data(const bytearr_t arr, pmd_reader_data_t * data);
int pmd_reader_read_data(const bytearr_t arr, pmd_reader_data_t * data);



#endif /* PMD_READER_H_ */
