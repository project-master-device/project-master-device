#include "device_layer.h"

uint8_t pmd_net_device_get_type(bytearr_t data) {
    if((data.itself != NULL) && (data.len > 0)) {
        switch(data.itself[0]) {
            case PMD_NET_DEVICE_TYPE_LED:
            case PMD_NET_DEVICE_TYPE_BUTTON:
            case PMD_NET_DEVICE_TYPE_READER:
                return data.itself[0];
        }
    }

    return 0;
}

uint8_t pmd_net_device_get_op(bytearr_t data) {
    if((data.itself != NULL) && (data.len > 1)) {
        return data.itself[1];
    }

    return 0;
}
