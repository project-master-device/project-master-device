#21.11.10

#   -HOWTO create a virtual can device:
#   -Probe the kernel modules. Don't forget to become root.
#depmod
#modprobe vcan
#modprobe can-raw
#    -Create a virtual CAN network interface for testing. Don't forget to become root.
#ip link add type vcan
#    -Create a virtual CAN network interface with a specific name 'can0'. Don't forget to become root.
#ip link add dev can0 type vcan
#    -Raise your new interface. Don't forget to become root.
#ifconfig can0 up
#

import sys
from node import can_net
import time
import datetime
import threading
import node.main

def print_handler(msg):
    print('print_handler:', msg)

s = can_net.CANserver("can0", print_handler)
#s.server_in.start()

#'12345678'.16 = -31-32-33-34-35-36-37-38-
msg_b = bytearray(b'12345678')
print('msg_b.size=%d', sys.getsizeof(msg_b))

#289 = 000000000000000000000000100100 | 0 | 0 | 1
service_inf = 289

print "send 1 time =", datetime.datetime.now()
bs = s.server_out.send((str(msg_b), can_id))
print(bs)

msg_b = bytearray(b'09876543')
print "send 2 time =", datetime.datetime.now()
bs = s.server_out.send((str(msg_b), can_id))
print(bs)

print('!-sleep 100ms-!')
time.sleep(0.1)

msg_b = bytearray(b'axcyezgk')
print "send 3 time =", datetime.datetime.now()
bs = s.server_out.send((str(msg_b), can_id))
print(bs)

s.server_in.shutdown()
