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


#include "contiki-conf.h"

#include "lowlevel.h"
#include "lib/ftimer.h"
#include "lib/interrupt.h"

static int enable = 0;

static void identification(void * ctx) {
    interrupt_disable();

    if(enable == 1) {
        led3_toggle();
        ftimer_register_func(identification, NULL, 50);
    }

    interrupt_enable();
}

void pmd_system_platform_identification_on() {
    if(enable == 0) {
        ftimer_register_func(identification, NULL, 50);
        enable = 1;
    }
}

void pmd_system_platform_identification_off() {
    if(enable == 1) {
        led3_off();
        enable = 0;
    }
}
