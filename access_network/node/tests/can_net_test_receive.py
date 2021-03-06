# Copyright (c) 2010-2011 Gennady Kalashnikov
#
# This file is part of Project Master Device.
#
# Project Master Device is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Project Master Device is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.


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

from node import can_net
import time
import datetime
import os
import threading
import node.main
from shared import log, config

def print_handler(msg):
    print 'print_handler msg str=<{0}> and recieve time={1}'.format(msg, datetime.datetime.now())

if __name__ == '__main__':
    print 'main: threading.current_thread().ident %u' % threading.current_thread().ident
    print 'main: os.getpid() %u' % os.getpid()
    print 'can net recieve delay = %f ms ' % (1000.0*float(config.config()['can_net']['recieve_delay']))
    s = can_net.CANserver("can0", print_handler)
    print "start time =", datetime.datetime.now()
    s.server_in.start()
    time.sleep(5)
    s.server_in.shutdown()
    print('__main__ END')
