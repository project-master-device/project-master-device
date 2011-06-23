#ifndef CAN_DRV_STRUCTS_H_
#define CAN_DRV_STRUCTS_H_

#include <stdint.h>

typedef struct {
	uint32_t id;
    uint8_t err;
    uint8_t rtr;
    uint8_t ide;
} can_frame_inf_t;

typedef struct {
    uint8_t itself[8];
    uint8_t len;
} can_frame_data_t;

typedef struct {
	can_frame_inf_t inf;
    can_frame_data_t data;
} can_frame_t;

typedef enum {
    CAN_20,
    CAN_50,
    CAN_100,
    CAN_125,
    CAN_250,
    CAN_500,
    CAN_800,
    CAN_1000
} can_speed_t;

#endif /* CAN_DRV_STRUCTS_H_ */
