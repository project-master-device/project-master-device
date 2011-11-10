/*
 * Copyright (c) 2010-2011 Gennady Kalashnikov
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


#ifndef PMD_BYTEARRAY_H_
#define PMD_BYTEARRAY_H_

#include <stdint.h>

/**
 * @struct bytearr_t
 * @brief simple byte array
 */
typedef struct {
	uint8_t* itself;
	uint32_t len;
} bytearr_t;

#endif // PMD_BYTEARRAY_H_
