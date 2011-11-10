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


#include "contiki.h"
#include "contiki-conf.h"
#include "net/can_drv.h"
#include "lib/ftimer.h"
#include "pmd_system.h"
#include "lowlevel.h"
#include <util/delay.h>

//PROCINIT(NULL);
PROCINIT(&can_drv_process, &etimer_process, &ftimer_process);

int main(void) {
    clock_init();

    process_init();
    procinit_init();
    lowlevel_init();
    pmd_system_init();

    int i;
    for(i = 0; i < 5; ++i) {
        led1_blink(1, 25);
        led2_blink(1, 25);
        led3_blink(1, 25);
    }

    autostart_start(autostart_processes);

    while (1) {
        process_run();
    }

    return 0;
}
