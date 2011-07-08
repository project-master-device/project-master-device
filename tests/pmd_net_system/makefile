CFLAGS = -Wall -Wextra -c -g
LFLAGS = -Wall -Wextra
COMMON_PATH = ../../common
SHARED_OBJS = config_core.o list.o system_config.o
CONFIG_OBJS = config.o $(SHARED_OBJS)

all: config

config: $(CONFIG_OBJS)
	gcc $(LFLAGS) $(CONFIG_OBJS) -o config

config.o: config.c $(COMMON_PATH)/pmd_net/system_ids.h
	gcc $(CFLAGS) config.c

config_core.o: $(COMMON_PATH)/lib/config_core.c $(COMMON_PATH)/lib/config_core.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/config_core.c

list.o: $(COMMON_PATH)/lib/list.c $(COMMON_PATH)/lib/list.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/list.c

system_config.o: $(COMMON_PATH)/pmd_net/system_config.h $(COMMON_PATH)/pmd_net/system_config.c
	gcc $(CFLAGS) $(COMMON_PATH)/pmd_net/system_config.c

clean:
	rm -rf *o config
