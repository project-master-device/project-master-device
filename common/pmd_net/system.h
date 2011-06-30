#ifndef PMD_NET_SYSTEM_H_
#define PMD_NET_SYSTEM_H_

#include "../net/can_net_middle.h"
#include "../lib/config_core.h"

#include <stdint.h>

#define PMD_NET_SYSTEM_CONFIG_REQUEST       1
#define PMD_NET_SYSTEM_CONFIG_FULL          2
#define PMD_NET_SYSTEM_CONFIG_SECTION_ADD   3
#define PMD_NET_SYSTEM_CONFIG_SECTION_DEL   4


typedef struct {
    uint8_t operation;
    config_cnf_t * config;
    config_section_t * section;
} pmd_net_system_config_data_t;

/**
 * @return
 *      0 -- successful write
 *      1 -- function parameters problem: data pointer is NULL
 *      2 -- memory allocation problem
 *      3 -- config full  command error: config pointer is NULL
 *      4 -- config full command error: failed to write config
 *      5 -- config section operation command error: section pointer is NULL
 *      6 -- config section add command error: failed to write section
 */
int pmd_net_system_config_write_data(bytearr_t arr, const pmd_net_system_config_data_t * data);

/**
 * @return
 *      0 -- successful read
 *      1 -- function parameters problem: data pointer is NULL or byte-array is not empty
 *      2 -- memory allocation problem
 *      3 -- config full command error: parsing failed
 *      4 -- config section add command error: parsing failed
 *      5 -- config section del command error: bad source data
 */
int pmd_net_system_config_read_data(const bytearr_t arr, pmd_net_system_config_data_t * data);

#endif /* PMD_NET_SYSTEM_H_ */
