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


'''
@package can_net module docstring
'''

import threading
import time
import traceback
import struct
import os

import can_socket
from shared import log, config

class IncomingCANserver(threading.Thread, object):
    '''
    @brief class-thread to recieve CAN messages as strings
    '''
    
    def __init__(self, socket, handler, timer_period=None):
        '''
        @param socket int: will use this can socket when receiving
        '''
        threading.Thread.__init__(self)
        self.daemon = True
        self._handler = handler
        self._socket = socket
        self.incomplete = {}
        if (timer_period != None):
            self._timer_period = timer_period
        else: 
            self._timer_period = config.config()['can_net']['recieve_delay']
    
    def shutdown(self):
        self._keep_working = False
    
    def run(self):
        '''
        @brief cycle saves message when its recieved
        '''
        self._keep_working = True
        log.logger().info('IncomingCANserver: started')
        while (self._keep_working):
            success = True
            try:
                msg = can_socket.recieve_some_data(self._socket)
                # can_socket module itself raises no exceptions, so make in here:
                if (msg == -1):
                    raise Exception('Error in can_socket.recieve_data args:('+str(self._socket)+')')
                elif (msg == None):
                    success = False
            except Exception as exc:
                success = False
                log.logger().error('IncomingCANserver: error while reading message: ' + traceback.format_exc())
            if (success):
                self._handler(msg)
            time.sleep(self._timer_period)


class OutcomingCANserver:
    '''
    @brief class to send CAN message as string
    '''
    CAN_MAX_DATA_LEN = 8
    def __init__(self, socket):
        '''
        @param socket int: will use this can socket when receiving
        '''
        self._socket = socket
        self._socket_lock = threading.Lock()
    
    
    def send(self, msg):
        '''
        @param serv_inf: int32 CAN_ID+EFF/RTR/ERR flags
        @param data: char[8]
        @return sent bytes
        can raise exception
        '''
        # in local protocols v0.2 msg len is conts: 8B
        serv_inf, data = msg
        if (len(data) > self.CAN_MAX_DATA_LEN):
            raise Exception ('data string is >8bytes: serv_inf={0}'.format(serv_inf, data))
        log.logger().debug('OutcomingCANserver: sending message: serv_inf={0}'.format(serv_inf, data))
        #TODO: confirmation
        
        self._socket_lock.acquire()
        try:
            bs = can_socket.send_frame(self._socket, data, serv_inf)
            # can_socket module itself raises no exceptions, so make in here:
            if (bs == -1):
                raise Exception('Error in can_socket.send_frame args:(socket={0}, serv_inf={1})'\
                                    .format(self._socket, serv_inf))
        finally:
            self._socket_lock.release()
        return bs


class CANserver:
    '''
    @brief class to send/recieve CAN message as string
    in - IncomingCANserver
    out - OutcomingCANserver
    '''
    
    def __init__(self, interface_name, handler, recieve_timer_period = None):
        '''
        @param interface_name string: interface name
        '''
        self._handler = handler
        self._interface_name = interface_name
        self._socket = can_socket.init_socket(interface_name)
        # can_socket module itself raises no exceptions, so make in here:
        if (self._socket == -1):
            raise Exception('Error in can_socket.init_socket args:('+interface_name+')')
        self.server_in = IncomingCANserver(self._socket, self._handler, recieve_timer_period)
        self.server_out = OutcomingCANserver(self._socket)

