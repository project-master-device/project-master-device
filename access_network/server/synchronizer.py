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
from threading import Thread
from shared.network import OutcomingConnServer
from shared import log, config

#Synchronize ALL nodes in one thread with one message - full db
class Synchronizer(Thread):
    def __init__(self, msg, conn):
        Thread.__init__(self)
        log.logger().info('Synchronization thread started.')
        self.__msg = msg
        self.__conn = conn
        self.__everything_OK = False

    def run(self):
        nodes = self.__msg.db.node
        tries = config.config()['server']['tries']
        while ((len(nodes) > 0) and (tries > 0)):
            problematic_nodes = []
            for node in nodes:
                if node.ip:
                    try:
                        log.logger().info('Synchronizer: processing node with id=%s, ip=%s', node.id, node.ip)
                        self.__conn.send_message((node.ip, config.config()['server']['nodes_port']), self.__msg.SerializeToString())
                        log.logger().info('Synchronizer: finished processing node with id=%s, ip=%s', node.id, node.ip)
                    except Exception as exc:
                        problematic_nodes.append(node)
                        log.logger().error('error while sending message: ' + repr(exc))
            if len(problematic_nodes) > 0: tries -= 1
            nodes = problematic_nodes
            sending_delay = config.config()['server']['sending_delay']
            time.sleep(sending_delay)
        if tries: self.__everything_OK = True

    def __del__(self):
        if self.__everything_OK:
            log.logger().info('Synchronizer: synchronization complete.') 
        else:
            log.logger().error('Synchronizer: something bad happened.')
        log.logger().info('Synchronization thread dead.')
