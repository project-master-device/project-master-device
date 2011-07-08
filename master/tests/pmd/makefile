OBJS_COMMON = list.o can_socket.o can_drv_lin.o can_protocols.o can_net_core.o can_net_middle.o can_net.o led.o reader.o button.o config.o config_core.o set_op.o heartbeat.o
OBJS_LED_COMMAND = $(OBJS_COMMON) led_command.o
OBJS_READER_LISTEN = $(OBJS_COMMON) reader_listen.o
OBJS_READER_COMMAND =  $(OBJS_COMMON) reader_command.o
OBJS_BUTTON_LISTEN = $(OBJS_COMMON) button_listen.o
OBJS_CONFIG_COMMAND = $(OBJS_COMMON) config_command.o
OBJS_SET_OP_MODE = $(OBJS_COMMON) set_op_mode.o
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

all: led_command reader_listen button_listen reader_command config_command set_op_mode heartbeat_receiver send_heartbeat


send_heartbeat: $(OBJS_SEND_HEARTBEAT)
	$(CC) -lpthread $(LFLAGS) $(OBJS_SEND_HEARTBEAT) -o send_heartbeat

send_heartbeat.o: send_heartbeat.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) send_heartbeat.c


heartbeat_receiver: $(OBJS_HEARTBEAT_RECEIVER)
	$(CC) -lpthread $(LFLAGS) $(OBJS_HEARTBEAT_RECEIVER) -o heartbeat_receiver

heartbeat_receiver.o: heartbeat_receiver.c $(NETPATH)can_net.h $(PMDNETPATH)system/system_ids.h
	$(CC) $(CFLAGS) heartbeat_receiver.c


set_op_mode: $(OBJS_SET_OP_MODE)
	$(CC) -lpthread $(LFLAGS) $(OBJS_SET_OP_MODE) -o set_op_mode

set_op_mode.o: set_op_mode.c $(NETPATH)can_net.h $(PMDNETPATH)system/system_ids.h
	$(CC) $(CFLAGS) set_op_mode.c


config_command: $(OBJS_CONFIG_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_CONFIG_COMMAND) -o config_command

config_command.o: config_command.c $(NETPATH)can_net.h $(PMDNETPATH)system/system_ids.h
	$(CC) $(CFLAGS) config_command.c


button_listen: $(OBJS_BUTTON_LISTEN)
	$(CC) -lpthread $(LFLAGS) $(OBJS_BUTTON_LISTEN) -o button_listen

button_listen.o: button_listen.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) button_listen.c


reader_command: $(OBJS_READER_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_READER_COMMAND) -o reader_command

reader_command.o: reader_command.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) reader_command.c


reader_listen: $(OBJS_READER_LISTEN)
	$(CC) -lpthread $(LFLAGS) $(OBJS_READER_LISTEN) -o reader_listen

reader_listen.o: reader_listen.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) reader_listen.c


led_command: $(OBJS_LED_COMMAND)
	$(CC) -lpthread $(LFLAGS) $(OBJS_LED_COMMAND) -o led_command

led_command.o: led_command.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) led_command.c


heartbeat.o: $(PMDNETPATH)system/heartbeat.h $(PMDNETPATH)system/heartbeat.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system/heartbeat.c

set_op.o: $(PMDNETPATH)system/set_op.h $(PMDNETPATH)system/set_op.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system/set_op.c

config.o: $(PMDNETPATH)system/config.h $(PMDNETPATH)system/config.c
	$(CC) $(CFLAGS) $(PMDNETPATH)system/config.c

button.o: $(PMDNETPATH)button/button.h $(PMDNETPATH)button/button.c
	$(CC) $(CFLAGS) $(PMDNETPATH)button/button.c

reader.o: $(PMDNETPATH)reader/reader.h $(PMDNETPATH)reader/reader.c
	$(CC) $(CFLAGS) $(PMDNETPATH)reader/reader.c

led.o: $(PMDNETPATH)led/led.h $(PMDNETPATH)led/led.c
	$(CC) $(CFLAGS) $(PMDNETPATH)led/led.c


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

config_core.o: $(LIBPATH)config_core.h $(LIBPATH)config_core.c
	$(CC) $(CFLAGS) $(LIBPATH)config_core.c


clean:
	rm -rf *o led_command reader_listen button_listen reader_command config_read config_command set_op_mode heartbeat_receiver send_heartbeat
