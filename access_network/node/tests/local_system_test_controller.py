#v1 - 25.11.10
#v2 - 16.12.10

'''
-HOWTO create a virtual can device:
sudo depmod
sudo modprobe vcan
sudo modprobe can-raw
sudo ip link add type vcan
sudo ip link add dev can0 type vcan
sudo ifconfig can0 up
'''

import sys
from node import can_net
import time
import datetime
import node.main
import struct


class virtual_controller:
    def __init__(self):
        timer_period = 0.001
        self.s = can_net.CANserver("can0", self.print_handler, timer_period)
        self.s.server_in.start()
    
    def print_handler(self, msg_str):
        msg_int = struct.unpack('Q', msg_str)
        print 'print_handler msg int=<{0}> and recieve time={1}'.format(msg_int[0], datetime.datetime.now())
        # SEND CONFIRMATION:
        # 000000000000000000000000000000 | 0 | 0 | 1
        serv_inf = 1
        # < 32 | 4 | 20 | 7 >
        # < response_code | device_id | hw_addr | protocol_version >
        # < 1, 1, 323, 0 >
        # < 0000 0000 0000 0000 0000 0000 0000 0000 0 | 0010 | 0000 0000 0001 0100 0011 | 0000000 >
        # < 0000000000000000000000000000000000010000000000001010000110000000 >
        data_1_int = 268476800
        bs = self._send(serv_inf, data_1_int)
    
    #TODO: test 1024bit key
    def scenario_1(self):
        print "-- SCENARIO_1 --: one message - 32bit key"
        print "send time =", datetime.datetime.now()
        
        # SEND:
        # 000000000000000000000000000000 | 0 | 0 | 1
        serv_inf = 1
        
        # < 32 | 1 | 4 | 20 | 7 >
        # < key | last_frame_bit | device_id | hw_addr | protocol_version >
        # < 123, 1, 1, 323, 1 >
        # < 0000 0000 0000 0000 0000 0000 0111 1011 | 1 | 0001 | 0000 0000 0001 0100 0011 | 0000001 >
        # < 0000000000000000000000000111101110001000000000001010000110000001 >
        data_1_int = 530562720129
        bs = self._send(serv_inf, data_1_int)
        print('bs = ', bs)
        print "Start waiting for response:".format(datetime.datetime.now())
        
    def _send(self, serv_inf, data_int):
        data_str = struct.pack('Q', data_int)
        bs = self.s.server_out.send((serv_inf, data_str))
        print('bs 1 = ', bs)
        
    def scenario_2(self, len):
        print "-- SCENARIO_2 --: many messages - length/32 bit key equal to 32bit key"
        print "send time =", datetime.datetime.now()
        if (len < 2): len = 2
        
        # SEND:
        # 000000000000000000000000000000 | 0 | 0 | 1
        serv_inf = 1
        
        # first frame:
        # < 32 | 1 | 4 | 20 | 7 >
        # < key | last_frame_bit | device_id | hw_addr | protocol_version >
        # < 123, 0, 1, 323, 1 >
        # < 0000 0000 0000 0000 0000 0000 0111 1011 | 0 | 0001 | 0000 0000 0001 0100 0011 | 0000001 >
        # < 0000000000000000000000000111101100001000000000001010000110000001 >
        self._send(serv_inf, 528415236481)
        # frames in middle:
        # < 0, 0, 1, 323, 1 >
        # < 0000 0000 0000 0000 0000 0000 0000 0000 | 0 | 0001 | 0000 0000 0001 0100 0011 | 0000001 >
        # < 0000000000000000000000000000000000001000000000001010000110000001 >
        for i in range(0, len - 2):
            self._send(serv_inf, 134259073)
        # last frame:
        # < 0, 1, 1, 323, 1 >
        # < 0000 0000 0000 0000 0000 0000 0000 0000 | 1 | 0001 | 0000 0000 0001 0100 0011 | 0000001 >
        # < 0000000000000000000000000000000010001000000000001010000110000001 >
        self._send(serv_inf, 2281742721)

        print "Start waiting for response:".format(datetime.datetime.now())

    
    def run(self):
        self.scenario_2(3) # will be processed
        time.sleep(1)
        self.scenario_1() # will be refused
        time.sleep(6)
        self.scenario_1() # will be processed
        time.sleep(6)
        self.s.server_in.shutdown()

if __name__ == '__main__':
    vc = virtual_controller()
    vc.run()
    print('__main__ END')
