OBJS_COMMON = list.o can_socket.o can_drv_lin.o can_protocols.o can_net_core.o can_net_middle.o can_net.o pmd_led.o pmd_reader.o pmd_button.o system_config.o config_core.o system_set_op.o
OBJS_LED_COMMAND = $(OBJS_COMMON) led_command.o
OBJS_READER = $(OBJS_COMMON) reader.o
OBJS_READER_COMMAND =  $(OBJS_COMMON) reader_command.o
OBJS_BUTTON = $(OBJS_COMMON) button.o
OBJS_CONFIG_READ = $(OBJS_COMMON) config_read.o
OBJS_CONFIG_COMMAND = $(OBJS_COMMON) config_command.o
OBJS_SET_OP = $(OBJS_COMMON) set_op.o
OBJS_HEARTBEAT_RECEIVER = $(OBJS_COMMON) heartbeat_receiver.o
OBJS_SEND_HEARTBEAT = $(OBJS_COMMON) send_heartbeat.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -DCAN_NET_LINUX
LFLAGS = -Wall $(DEBUG)
LNETPATH = ../../local_net_CAN/
NETPATH = ../../../common/net/
LIBPATH = ../../../common/lib/
PMDNETPATH = ../../../common/pmd_net/

all: led_command reader button reader_command config_read config_command set_op heartbeat_receiver send_heartbeat


send_heartbeat: $(OBJS_SEND_HEARTBEAT)
	$(CC) -lpthread $(LFLAGS) $(OBJS_SEND_HEARTBEAT) -o send_heartbeat

send_heartbeat.o: send_heartbeat.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) send_heartbeat.c


heartbeat_receiver: $(OBJS_HEARTBEAT_RECEIVER)
	$(CC) -lpthread $(LFLAGS) $(OBJS_HEARTBEAT_RECEIVER) -o heartbeat_receiver

heartbeat_receiver.o: heartbeat_receiver.c $(NETPATH)can_net.h $(PMDNETPATH)system_ids.h
	$(CC) $(CFLAGS) heartbeat_receiver.c


set_op: $(OBJS_SET_OP)
	$(CC) -lpthread $(LFLAGS) $(OBJS_SET_OP) -o set_op

set_op.o: set_op.c $(NETPATH)can_net.h $(PMDNETPATH)system_ids.h
	$(CC) $(CFLAGS) set_op.c


config_command: $(OBJS_CONFIG_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_CONFIG_COMMAND) -o config_command

config_command.o: config_command.c $(NETPATH)can_net.h $(PMDNETPATH)system_ids.h
	$(CC) $(CFLAGS) config_command.c


config_read: $(OBJS_CONFIG_READ)
	$(CC) -lpthread $(LFLAGS) $(OBJS_CONFIG_READ) -o config_read

config_read.o: config_read.c $(NETPATH)can_net.h $(PMDNETPATH)system_ids.h
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

system_set_op.o: $(PMDNETPATH)system_set_op.h $(PMDNETPATH)system_set_op.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system_set_op.c

system_config.o: $(PMDNETPATH)system_config.h $(PMDNETPATH)system_config.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system_config.c

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
	rm -rf *o led_command reader button reader_command config_read config_command set_op heartbeat_receiver send_heartbeat
