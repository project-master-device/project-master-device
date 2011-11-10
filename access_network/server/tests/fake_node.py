# Copyright (c) 2010-2011 Sergey Gnuskov
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
from shared.network import *
from shared import man_db_msg_pb2 as proto
import server.main
from shared import database

def fake_node(msg):
    print 'Recieved message;\n', msg

if __name__ == '__main__':
    try:
        net = ConnServer(("localhost", 8566), {'new_db': fake_node})
        net.start_servers()
        
        log = proto.man_message()
        log.message_type = 'log'
        database.manager().get_log(log.db)
        time.sleep(10)
        print "sending log to server..."
        net.server_out.send_message(("localhost", 8563), log.SerializeToString())
        time.sleep(5)
        net.shutdown()
        
    except Exception as exc:
        print('ERR:', repr(exc))
        net.shutdown()
