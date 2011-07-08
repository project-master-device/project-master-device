#ifndef PMD_NET_READER_H_
#define PMD_NET_READER_H_

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
    uint8_t data[PMD_READER_MSG_LEN];
} pmd_net_reader_data_t;

int pmd_net_reader_write_data(bytearr_t* arr, const pmd_net_reader_data_t * data);
int pmd_net_reader_read_data(const bytearr_t* arr, pmd_net_reader_data_t * data);

#endif /* PMD_NET_READER_H_ */
