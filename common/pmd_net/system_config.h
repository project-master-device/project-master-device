#ifndef PMD_NET_SYSTEM_H_
#define PMD_NET_SYSTEM_H_

#include "../net/can_net_middle.h"
#include "../lib/config_core.h"

#include <stdint.h>


typedef struct {
    uint8_t operation;
    config_cnf_t * config;
    config_section_t * section;
} pmd_net_system_config_data_t;

// Config message operations
#define PMD_NET_SYSTEM_CONFIG_REQUEST       1
#define PMD_NET_SYSTEM_CONFIG_FULL          2
#define PMD_NET_SYSTEM_CONFIG_SECTION_ADD   3
#define PMD_NET_SYSTEM_CONFIG_SECTION_DEL   4

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source data or destination array pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- config full command error: config pointer is NULL
 *      4 -- config full command error: failed to write config
 *      5 -- config section operation command (add/del) error: section pointer is NULL
 *      6 -- config section add command error: failed to write section
 */
int pmd_net_system_config_write_data(bytearr_t * dest_arr, const pmd_net_system_config_data_t * source_data);

/**
 * @return
 *      0 -- successful
 *      1 -- function parameters problem: source array or destination data pointer are NULL
 *      2 -- memory allocation problem
 *      3 -- bad source data
 *      4 -- config full command error: parsing failed
 *      5 -- config section add command error: parsing failed
 */
int pmd_net_system_config_read_data(const bytearr_t * source_arr, pmd_net_system_config_data_t * dest_data);

#endif /* PMD_NET_SYSTEM_H_ */
