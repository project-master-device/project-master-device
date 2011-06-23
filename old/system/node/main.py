import os
from shared import log, config, database
from node.Node import Node

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