#include "ftimer.h"
#include "contiki-conf.h"
#include "lib/list.h"
#include "lib/malloc.h"
#include <stddef.h>

typedef struct {
    void * next;

    uint8_t id;
    void (*func)(void *);
    void * context;
    double time;
} ftimer_t;

static uint32_t funcs = 0;
LIST(ft_functions);

PROCESS(ftimer_process, "Process for periodic functions calls");

int ftimer_register_func(void (*func)(void *), void * context, uint16_t ms) {
    ftimer_t * ft = (ftimer_t *)malloc(sizeof(ftimer_t));
    int i, id = -1;

    for (i = 0; i < 32; i++) {
        if ((funcs & (1 << i)) == 0) {
            id = i;
            funcs |= (1 << id);
            break;
        }
    }
    if (id == -1)  return -1;

    ft->id = id;
    ft->func = func;
    ft->context = context;
    ft->time = ms * ((double)CLOCK_CONF_SECOND / 1000.0);
    list_add(ft_functions, (void *)ft);
    return ft->id;
}

void ftimer_unregister_func(uint8_t id) {
    ftimer_t * ft;
    for (ft = (ftimer_t *)list_head(ft_functions); ft != NULL; ft = (ftimer_t *)list_item_next((void *)ft)) {
        if (ft->id == id) {
            list_remove(ft_functions, (void *)ft);
            free(ft);
            funcs &= ~(1 << id);
            break;
        }
    }
}

PROCESS_THREAD(ftimer_process, ev, data) {
    static struct etimer et;
    ftimer_t * ft;
    ftimer_t * next_ft;

    PROCESS_BEGIN();

    etimer_set(&et, FTIMER_Q);
    list_init(ft_functions);

    for (;;) {
        PROCESS_WAIT_EVENT();
        for(ft = (ftimer_t *)list_head(ft_functions); ft != NULL;) {
            ft->time -= FTIMER_Q;
            next_ft = (ftimer_t *)list_item_next((void *)ft);
            if (ft->time < FTIMER_Q) {
                ft->func(ft->context);
                ftimer_unregister_func(ft->id);
            }
            ft = next_ft;
        }
        etimer_reset(&et);
    }

    PROCESS_END();
}

