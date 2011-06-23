#!/bin/bash
sudo depmod
sudo modprobe vcan
sudo modprobe can-raw
# Create a virtual CAN network interface for testing. Don't forget to become root.
sudo ip link add type vcan
# Create a virtual CAN network interface with a specific name 'can0'. Don't forget to become root.
sudo ip link add dev can0 type vcan
# Raise your new interface. Don't forget to become root.
sudo ifconfig can0 up
