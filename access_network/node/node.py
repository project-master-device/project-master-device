# Copyright (c) 2010-2011 Gennady Kalashnikov, Timofey Kirillov
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


import time
import traceback
import struct

from shared.network import ConnServer
from shared import log, config, database as db
from can_net import CANserver
from points_manager import PointsManager
from log_synchronizer import LogSynchronizer
import local_protocols as lprotocols

#TCP mesage handlers:
def save_db_handler(msg):
    try:
        log.logger().info('got new db, synchronizing...')
        with db.manager().transaction():
            db.manager().drop_sync_data()
            db.manager().load_sync_data(msg.db)

        log.logger().info('successfully synchronized')

    except Exception, exc:
        log.logger().error('failed to reset local database: ' + traceback.format_exc())

class Node:
    def __init__(self):
        self._tcp_port = config.config()['network']['port']
        tcp_handlers = {'new_db': save_db_handler}
        self._tcp_net = ConnServer(("localhost", self._tcp_port), tcp_handlers)
        self._local_net = CANserver(config.config()['can_net']['interface'], self.local_message_handler)
        self._conf_period = config.config()['can_net']['confirmation_period']
        # number of periods to wait:
        self._conf_timeout = config.config()['can_net']['confirmation_timeout'] / self._conf_period
        self._conf_waiters = {}
        # _local_handlers struct: (message hanler, message decoder)
        self._local_handlers = {0: (self._handle_confirmation_msg, lprotocols.in_lvl2_confirmation), \
                                1: (self._handle_common_msg, lprotocols.in_lvl2_common)}
        self._points_manager = PointsManager(self.send_local_msg)
        self._log_sync = LogSynchronizer(self._tcp_net.server_out)
    
    #### <local_handlers>
    def _handle_confirmation_msg(self, hw_addr, device_id, response_code):
        #TODO: log smth
        self._conf_waiters[(hw_addr, device_id)] = response_code
    
    def _handle_common_msg(self, hw_addr, device_id, lfb, data):
        # concatenate msgs (msgs must always be sended&recieved in order)
        id = (hw_addr, device_id)
        buffer = self._local_net.server_in.incomplete
        if (not buffer.has_key(id)):
            buffer[id] = []
        buffer[id].append(data)
        if (lfb == 1):
                self._points_manager.process_msg(id[0], id[1], buffer[id])
                buffer[id] = []
    #### <\local_handlers>
    
    def send_local_msg(self, id, msg):
        self._local_net.server_out.send(msg)
        for i in range(0, int(self._conf_timeout)):
            time.sleep(self._conf_period)
            if self._conf_waiters.has_key(id):
                return self._conf_waiters.pop(id)
        return None
        
    def start(self):
        self._tcp_net.start_servers()
        self._local_net.server_in.start()
        self._log_sync.run() # ~ this is Node loop
        #while True:
    
    def local_message_handler(self, msg_str):
        try:
            msg_int = struct.unpack('Q', msg_str)
            version, data = lprotocols.in_lvl1.unpack_msg(msg_int[0])
            handler = self._local_handlers[version]
            args = handler[1].unpack_msg(data)
            handler[0](*args)
            
        except Exception as exc:
            log.logger().error('failed to handle local message: ' + traceback.format_exc())
    
    def stop(self):
        self._tcp_net.shutdown()
        log.logger().info('Node stopped.')
