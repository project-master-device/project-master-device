# $$

all: contiki-avr-mb128-main.elf #loadable_prg.ko
upload: contiki-avr-mb128-main.eep contiki-avr-mb128-main.u

TARGET=avr-mb128

#define APPS in eclipse Make target entry
#APPS+=mb128-webserver 
#APPS+=mb128-remotecontrol
#APPS+=mb128-cantest
#APPS+=mb128-canconverter
#APPS+=mb128-testprocess

CONTIKI=../..
include $(CONTIKI)/Makefile.include
