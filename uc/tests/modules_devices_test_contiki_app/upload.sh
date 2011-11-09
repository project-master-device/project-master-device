#!/bin/bash
sudo make modules_devices_test_contiki_app.avr-our.u
if [ "$?" -eq "0" ]; then
	sudo avrdude -v -P /dev/ttyUSB0 -c duemilanove -p at90can128 -U eeprom:w:file.hex
fi
	
	
	
	
