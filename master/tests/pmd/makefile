OBJS_COMMON = list.o can_socket.o can_drv_lin.o can_protocols.o can_net_core.o can_net_middle.o can_net.o pmd_led.o pmd_reader.o pmd_button.o system.o config_core.o
OBJS_LED_COMMAND = $(OBJS_COMMON) led_command.o
OBJS_READER = $(OBJS_COMMON) reader.o
OBJS_READER_COMMAND =  $(OBJS_COMMON) reader_command.o
OBJS_BUTTON = $(OBJS_COMMON) button.o
OBJS_CONFIG_READ = $(OBJS_COMMON) config_read.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -DCAN_NET_LINUX
LFLAGS = -Wall $(DEBUG)
LNETPATH = ../../local_net_CAN/
NETPATH = ../../../common/net/
LIBPATH = ../../../common/lib/
PMDNETPATH = ../../../common/pmd_net/

all: led_command reader button reader_command config_read

config_read: $(OBJS_CONFIG_READ)
	$(CC) -lpthread $(LFLAGS) $(OBJS_CONFIG_READ) -o config_read

config_read.o: config_read.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) config_read.c

button: $(OBJS_BUTTON)
	$(CC) -lpthread $(LFLAGS) $(OBJS_BUTTON) -o button

button.o: button.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) button.c

reader_command: $(OBJS_READER_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_READER_COMMAND) -o reader_command

reader_command.o: reader_command.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) reader_command.c

reader: $(OBJS_READER)
	$(CC) -lpthread $(LFLAGS) $(OBJS_READER) -o reader

reader.o: reader.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) reader.c

led_command: $(OBJS_LED_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_LED_COMMAND) -o led_command

led_command.o: led_command.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) led_command.c


config_core.o: $(LIBPATH)config_core.h $(LIBPATH)config_core.c
	$(CC) $(CFLAGS) $(LIBPATH)config_core.c

system.o: $(PMDNETPATH)system.h $(PMDNETPATH)system.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system.c

pmd_button.o: $(PMDNETPATH)pmd_button.h $(PMDNETPATH)pmd_button.c
	$(CC) $(CFLAGS) $(PMDNETPATH)pmd_button.c

pmd_reader.o: $(PMDNETPATH)pmd_reader.h $(PMDNETPATH)pmd_reader.c
	$(CC) $(CFLAGS) $(PMDNETPATH)pmd_reader.c

pmd_led.o: $(PMDNETPATH)pmd_led.h $(PMDNETPATH)pmd_led.c
	$(CC) $(CFLAGS) $(PMDNETPATH)pmd_led.c

can_net.o: $(NETPATH)can_net.h $(NETPATH)can_net.c $(NETPATH)can_net_middle.h
	$(CC) $(CFLAGS) -lpthread $(NETPATH)can_net.c

can_net_middle.o: $(NETPATH)can_net_middle.h $(NETPATH)can_net_middle.c $(NETPATH)can_net_core.h
	$(CC) $(CFLAGS) $(NETPATH)can_net_middle.c

can_net_core.o: $(NETPATH)can_net_core.h $(NETPATH)can_net_core.c $(NETPATH)can_protocols.h $(NETPATH)can_drv.h $(LIBPATH)list.h
	$(CC) $(CFLAGS) $(NETPATH)can_net_core.c

can_protocols.o: $(NETPATH)can_protocols.h $(NETPATH)can_protocols.c $(NETPATH)can_drv_structs.h
	$(CC) $(CFLAGS) $(NETPATH)can_protocols.c

can_drv_lin.o: $(NETPATH)can_drv.h $(LNETPATH)can_drv_lin.c $(LNETPATH)can_socket.h $(LIBPATH)list.h
	$(CC) -lpthread $(CFLAGS) $(LNETPATH)can_drv_lin.c

can_socket.o: $(LNETPATH)can_socket.h $(LNETPATH)can_socket.c
	$(CC) $(CFLAGS) $(LNETPATH)can_socket.c

list.o: $(LIBPATH)list.h $(LIBPATH)list.c
	$(CC) $(CFLAGS) $(LIBPATH)list.c

clean:
	rm -rf *o led_command reader button reader_command config_read
