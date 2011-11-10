# Copyright (c) 2010-2011 Sergey Gnuskov, Timofey Kirillov
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


import os
from shared import log, config, database
from node.node import Node

default_log_conf_path = os.path.dirname(os.path.abspath(__file__)) + '/../config/node_logging.conf'
default_conf_path = os.path.dirname(os.path.abspath(__file__)) + '/../config/node_config.yml'

log.initialize(default_log_conf_path)
config.initialize(default_conf_path)

db_connection_string = 'sqlite:' + os.path.abspath(config.config()['database']['filename'])
db_debug_mode = config.config()['database']['debug']

database.initialize(db_connection_string, db_debug_mode)

if __name__ == '__main__':
    try:
        node = Node()
        node.start()
    except KeyboardInterrupt:
        log.logger().warning('Stopping node on user command.')
        node.stop()
