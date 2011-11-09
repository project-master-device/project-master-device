CONTIKI_PROJECT = blink_contiki_app
all: $(CONTIKI_PROJECT)

CONTIKI = ../../trunk

CONTIKI_BACKYARD_TREE = on
CONTIKI_LOCAL_TREE = on

AVRDUDE_OPTIONS =  -v
AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_PROGRAMMER = duemilanove

include $(CONTIKI)/Makefile.include
