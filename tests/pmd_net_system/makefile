CFLAGS = -Wall -Wextra -g -c
LFLAGS = -Wall -Wextra
COMMON_PATH = ../../common
SHARED_OBJS = config_core.o list.o config.o
CONF_OBJS = conf.o $(SHARED_OBJS)

all: conf

conf: $(CONF_OBJS)
	gcc $(LFLAGS) $(CONF_OBJS) -o conf

conf.o: conf.c $(COMMON_PATH)/pmd_net/system/system_ids.h
	gcc $(CFLAGS) conf.c

config_core.o: $(COMMON_PATH)/lib/config_core.c $(COMMON_PATH)/lib/config_core.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/config_core.c

list.o: $(COMMON_PATH)/lib/list.c $(COMMON_PATH)/lib/list.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/list.c

config.o: $(COMMON_PATH)/pmd_net/system/config.h $(COMMON_PATH)/pmd_net/system/config.c
	gcc $(CFLAGS) $(COMMON_PATH)/pmd_net/system/config.c

clean:
	rm -rf *o conf
