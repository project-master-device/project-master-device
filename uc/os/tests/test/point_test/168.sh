#!/bin/sh
MCU=atmega168 make
sudo ../utils/upload main /dev/`dmesg | grep -o 'ttyUSB[0-9]\+' | tail -1` arduino
make clean
