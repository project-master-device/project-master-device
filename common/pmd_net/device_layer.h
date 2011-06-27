#ifndef DEVICE_LAYER_H_
#define DEVICE_LAYER_H_

#include "../net/can_net_middle.h"

#define PMD_NET_DEVICE_TYPE_LED     1
#define PMD_NET_DEVICE_TYPE_BUTTON  2
#define PMD_NET_DEVICE_TYPE_READER  3

#define PMD_NET_DEVICE_LED_ON       1
#define PMD_NET_DEVICE_LED_OFF      2
#define PMD_NET_DEVICE_LED_TOGGLE   3

#define PMD_NET_DEVICE_BUTTON_DOWN  1
#define PMD_NET_DEVICE_BUTTON_UP    2

#define PMD_NET_DEVICE_READER_CODE  1

uint8_t pmd_net_device_get_type(bytearr_t data);
uint8_t pmd_net_device_get_op(bytearr_t data);

#endif /* DEVICE_LAYER_H_ */
