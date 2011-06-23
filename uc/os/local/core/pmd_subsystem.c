#include "pmd_subsystem.h"

#include "dev/button.h"
#include "lib/device.h"
#include "lib/interrupt.h"


unsigned pmd_init() {
    /* Init config */
    config_init();
    if (config_open() == -1)
        return 0;

    /* Establish connection to master */
    //can_net_establish();

    /* Enable interrupts */
//    interrupts_init();

    /* Init devices */
    if (!devices_init())
        return 0;

    return 1;
}

