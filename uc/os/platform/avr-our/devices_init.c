#include "lib/device.h"
#include "lib/config.h"
#include "lib/malloc.h"
#include "lib/interrupt.h"
#include "sys/process.h"

#include "dev/led.h"
#include "dev/button.h"
#include "dev/reader.h"

#include <string.h>


unsigned devices_init() {
    config_section_t * sect;
    config_cnf_t * cnf = config_get();
    char type[16];

    if (cnf == NULL) return 0;


    for (sect = (config_section_t *)list_head(cnf->sections); sect != NULL; sect = (config_section_t *)list_item_next((void*)sect)) {
        config_section_get_str(sect, "type", NULL, type, strlen(type));
        if (type == NULL)
            return 0;

        if (strcmp(type, "led") == 0) {
            led_create(sect);
        } else if (strcmp(type, "button") == 0) {
            button_create(sect);
        } else if (strcmp(type, "reader") == 0) {
            reader_create(sect);
        }
        // etc. new drivers

    }


    return 1;
}


