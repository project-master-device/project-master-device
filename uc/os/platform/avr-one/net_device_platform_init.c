#include "contiki-conf.h"
#include "lib/net_device.h"
#include "lib/malloc.h"

#include "dev/led.h"
#include "dev/button.h"
#include "dev/reader.h"

#include <string.h>

int net_device_platform_init() {
    config_section_t * sect = NULL;
    net_device_t * net_device = NULL;
    char type[16];

    config_cnf_t * cnf = config_get();
    if(cnf == NULL) return 1;

    for(sect = (config_section_t *)list_head(cnf->sections); sect != NULL; sect = (config_section_t *)list_item_next((void*)sect)) {
        config_section_get_str(sect, "type", NULL, type, strlen(type));
        if (type == NULL)
            return 2;

        if (strcmp(type, "led") == 0) {
            if(led_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, led_net_callback);

        } else if (strcmp(type, "button") == 0) {
            if(button_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, NULL);

        } /*else if (strcmp(type, "reader") == 0) {
            if(reader_init(sect) != 0)
                return 3;

            net_device = (net_device_t *)malloc(sizeof(net_device_t));
            if(net_device == NULL)
                return 4;
            net_device_construct(net_device, sect, NULL);
        }*/
        // etc. new drivers
    }

    return 0;
}
