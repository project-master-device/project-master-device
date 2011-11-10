/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov, Timofey Kirillov
 *
 * This file is part of Project Master Device.
 *
 * Project Master Device is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Project Master Device is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


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
