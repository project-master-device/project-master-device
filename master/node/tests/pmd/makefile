OBJS = list.o can_socket.o can_drv_lin.o can_protocols.o can_net_core.o can_net_middle.o can_net.o pmd_led.o led_command.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -DCAN_NET_LINUX
LFLAGS = -Wall $(DEBUG)
LNETPATH = ../../local_net_CAN/
NETPATH = ../../../../common/net/
PMDNETPATH = ../../../../common/pmd_net/

all: led_command

led_command: $(OBJS)
	$(CC) -lpthread $(LFLAGS) $(OBJS) -o led_command

led_command.o: led_command.c $(NETPATH)can_net.h
	$(CC) $(CFLAGS) led_command.c

pmd_led.o: $(PMDNETPATH)pmd_led.h $(PMDNETPATH)pmd_led.c
	$(CC) $(CFLAGS) $(PMDNETPATH)pmd_led.c

can_net.o: $(NETPATH)can_net.h $(NETPATH)can_net.c $(NETPATH)can_net_middle.h
	$(CC) $(CFLAGS) -lpthread $(NETPATH)can_net.c

can_net_middle.o: $(NETPATH)can_net_middle.h $(NETPATH)can_net_middle.c $(NETPATH)can_net_core.h
	$(CC) $(CFLAGS) $(NETPATH)can_net_middle.c

can_net_core.o: $(NETPATH)can_net_core.h $(NETPATH)can_net_core.c $(NETPATH)can_protocols.h $(NETPATH)can_drv.h $(NETPATH)list.h
	$(CC) $(CFLAGS) $(NETPATH)can_net_core.c

can_protocols.o: $(NETPATH)can_protocols.h $(NETPATH)can_protocols.c $(NETPATH)can_drv_structs.h
	$(CC) $(CFLAGS) $(NETPATH)can_protocols.c

can_drv_lin.o: $(NETPATH)can_drv.h $(LNETPATH)can_drv_lin.c $(LNETPATH)can_socket.h $(NETPATH)list.h
	$(CC) -lpthread $(CFLAGS) $(LNETPATH)can_drv_lin.c

can_socket.o: $(LNETPATH)can_socket.h $(LNETPATH)can_socket.c
	$(CC) $(CFLAGS) $(LNETPATH)can_socket.c

list.o: $(NETPATH)list.h $(NETPATH)list.c
	$(CC) $(CFLAGS) $(NETPATH)list.c

clean:
	rm -rf *o led_command
