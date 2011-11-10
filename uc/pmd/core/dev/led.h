/*
 * Copyright (c) 2010-2011 Sergey Gnuskov, Timofey Kirillov
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


/**
 * @file led.h
 * @brief A driver for led device.
 */

#ifndef LED_H_
#define LED_H_

#include "lib/config.h"
#include "net/can_net.h"

int led_init(config_section_t * conf_sect);
void led_net_callback(config_section_t * conf_sect, const msg_lvl2_t * net_msg);

#endif /* LED_H_ */
