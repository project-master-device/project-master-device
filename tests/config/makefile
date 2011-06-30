CFLAGS = -Wall -Wextra -c
LFLAGS = -Wall -Wextra
COMMON_PATH = ../../common
SHARED_OBJS = config_core.o list.o
CONFIG_TEST_OBJS = config_test.o $(SHARED_OBJS)

all: config_test

config_test: $(CONFIG_TEST_OBJS)
	gcc $(LFLAGS) $(CONFIG_TEST_OBJS) -o config_test

config_test.o: config_test.c
	gcc $(CFLAGS) config_test.c

config_core.o: $(COMMON_PATH)/lib/config_core.c $(COMMON_PATH)/lib/config_core.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/config_core.c

list.o: $(COMMON_PATH)/lib/list.c $(COMMON_PATH)/lib/list.h
	gcc $(CFLAGS) $(COMMON_PATH)/lib/list.c

clean:
	rm -rf *o config_test
