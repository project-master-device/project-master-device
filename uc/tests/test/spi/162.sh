#!/bin/sh
MCU=atmega162 make
sudo ../utils/upload main /dev/`dmesg | grep 'FTDI.*ttyUSB' | tail -1 | cut -d ' ' -f 12` v2
make clean
