# Copyright (c) 2010-2011 Timofey Kirillov
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


# -*- coding: UTF-8 -*-

from threading import Thread
import time

import shared.database as db
from shared import man_db_msg_pb2 as proto
from shared import config
from shared import log
from node.exceptions import LogSyncError

class LogSynchronizer(Thread):
    def __init__(self, out_conn_server):
        Thread.__init__(self)
        self.__conn = out_conn_server
        self.daemon = True

    def run(self):
        tries = config.config()['log_synchronization']['tries']
        period = 60*config.config()['log_synchronization']['period']
        server_ip = config.config()['network']['server_ip']
        server_port = config.config()['network']['server_port']
        while True:
            try:
                with db.manager().transaction():
                    log.logger().info('Sending log to server...')
                    _log = proto.man_message()
                    _log.message_type = 'log'
                    db.manager().get_log(_log.db)
                    while tries > 0:
                        try:
                            self.__conn.send_message((server_ip, server_port), _log.SerializeToString())
                            break
                        except Exception as exc:
                            log.logger().error('Log synchronizer: ' + repr(exc))
                            tries -= 1
                    if tries > 0: 
                        db.manager().drop_log()
                        log.logger().info('Log successfully sent.')
                    else: 
                        raise LogSyncError('Log sending failed.')
            except LogSyncError as err:
                log.logger().info(err.value)
            except Exception as exc:
                log.logger().error(repr(exc))
            tries = config.config()['log_synchronization']['tries']
            time.sleep(period)
