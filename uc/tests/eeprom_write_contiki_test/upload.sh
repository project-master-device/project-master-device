#!/bin/bash
sudo make eeprom_write_contiki_test.avr-our.u
if [ "$?" -eq "0" ]; then
	sudo avrdude -v -P /dev/ttyUSB0 -c duemilanove -p at90can128 -U eeprom:w:file.hex
fi
	
	
	
	
