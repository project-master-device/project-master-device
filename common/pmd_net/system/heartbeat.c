#include "heartbeat.h"
#include <stddef.h>
#include <stdlib.h>

int pmd_net_sys_heartbeat_write_data(bytearr_t* data) {
	data->len = 0;
	data->itself = NULL;

    return 0;
}
