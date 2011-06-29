#include <stdio.h>
#include <string.h>

#include "../../common/lib/config_core.h"

int main() {
    config_cnf_t config;
    config_cnf_construct(&config);
    int rc;
    config_section_t * sect = NULL;
    config_int_t i;
    config_uint_t ui;
    char str_buf[256];

    for(i = 0; i < 256; ++i)
        str_buf[i] = '\0';

    const char * orig_str = "[1]\n\
key1=i100\n\
key2=i100\n\
key3=sqwerty\n\
[2]\n\
key1=I123\n\
key2=i-100\n\
key3=sqwerty\n\
wererwer=s101010100\n\
~";

    rc = config_parse(orig_str, &config);

    if(rc == 0) {
        sect = (config_section_t *)list_head(config.sections);

        if(sect->id != 1) {
            printf("[FAIL] section id mismatch -> '%d != 1' -> %s:%d\n", sect->id, __FILE__, __LINE__);
            return 1;
        }

        i = config_section_get_int(sect, "key1", -1);
        if(i != 100) {
            printf("[FAIL] section 1 -> key1 check failed -> '%d != 100' -> %s:%d\n", i, __FILE__, __LINE__);
            return 1;
        }

        i = config_section_get_int(sect, "key2", -1);
        if(i != 100) {
            printf("[FAIL] section 1 -> key2 check failed -> '%d != 100' -> %s:%d\n", i, __FILE__, __LINE__);
            return 1;
        }

        config_section_get_str(sect, "key3", "fail", str_buf, 256);
        if(strcmp(str_buf, "qwerty") != 0) {
            printf("[FAIL] section 1 -> key3 check failed -> '%s != qwerty' -> %s:%d\n", str_buf, __FILE__, __LINE__);
        }

        sect = (config_section_t *)list_item_next((void *)sect);

        if(sect->id != 2) {
            printf("[FAIL] section id mismatch -> '%d != 2' -> %s:%d\n", sect->id, __FILE__, __LINE__);
            return 1;
        }

        ui = config_section_get_uint(sect, "key1", 222);
        if(ui != 123) {
            printf("[FAIL] section 2 -> key1 check failed -> '%u != 123' -> %s:%d\n", i, __FILE__, __LINE__);
            return 1;
        }

        i = config_section_get_int(sect, "key2", -1);
        if(i != -100) {
            printf("[FAIL] section 2 -> key2 check failed -> '%d != -100' -> %s:%d\n", i, __FILE__, __LINE__);
            return 1;
        }

        config_section_get_str(sect, "key3", "fail", str_buf, 256);
        if(strcmp(str_buf, "qwerty") != 0) {
            printf("[FAIL] section 2 -> key3 check failed -> '%s != qwerty' -> %s:%d\n", str_buf, __FILE__, __LINE__);
            return 1;
        }

        config_section_get_str(sect, "wererwer", "fail", str_buf, 256);
        if(strcmp(str_buf, "101010100") != 0) {
            printf("[FAIL] section 2 -> wererwer check failed -> '%s != 101010100' -> %s:%d\n", str_buf, __FILE__, __LINE__);
            return 1;
        }

        printf("config parse success\n");
    } else {
        printf("[FAIL] config parse failed -> %s:%d\n", __FILE__, __LINE__);
        return 1;
    }

    char * new_str = config_emit(&config);

    unsigned int len = strlen(new_str);
    if(len != strlen(orig_str)) {
        printf("[FAIL] original config string length not equals to emitted string -> %s:%d\n", __FILE__, __LINE__);
        return 1;
    }

    if(memcmp((void *)orig_str, (void *)new_str, len + 1) != 0) {
        printf("[FAIL] original config string not equals to emitted string -> %s:%d\n", __FILE__, __LINE__);
        return 1;
    }

    printf("config emit success\n");

    config_cnf_destruct(&config);

    printf("good\n");
    return 0;
}
