#x3
#14.11.2010
#===============================================================================
# Some information
#===============================================================================

import os, time
from shared import log, config, database
from server.synchronizer import Synchronizer
from shared.network import *
import shared.man_db_msg_pb2 as proto


default_log_conf_path = os.path.dirname(os.path.abspath(__file__)) + '/../config/server_logging.conf'
default_conf_path = os.path.dirname(os.path.abspath(__file__)) + '/../config/server_config.yml'

log.initialize(default_log_conf_path)
config.initialize(default_conf_path)

db_connection_string = 'mysql://' + config.config()['database']['user'] + ':' + config.config()['database']['password'] + '@' + config.config()['database']['host'] + '/' + config.config()['database']['db']
db_debug_mode = config.config()['database']['debug']

database.initialize(db_connection_string, db_debug_mode)
        
    
def log_handler(msg):
    try:
        log.logger().info('Got log, synchronizing...')
        with database.manager().transaction():
            database.manager().load_log(msg.db)
            log.logger().info('Log successfully synchronized.')
    except Exception as exc:
        log.logger().error(repr(exc))
        


class Server():
    def __init__(self):
        self.__servers = ConnServer(('localhost', config.config()['server']['port']), {'log': log_handler})

    #Start server
    #'Synchronize ALL in one thread' version
    def start(self):
        try:
            log.logger().info('Server started.')
            self.__servers.start_servers()
            
            while True:
                wait = 60 * float(config.config()['server']['sync_period'])
                
                new_db = proto.man_message()
                new_db.message_type = 'new_db'
                database.manager().get_sync_data(new_db.db)
                sync = Synchronizer(new_db, self.__servers.server_out).start()
                time.sleep(wait)
                
        except KeyboardInterrupt:
            log.logger().warning('Stopping server on user command.')
        except Exception as exc:
            log.logger().error(repr(exc))
                        
        finally: self.stop()
        
    def stop(self):
        self.__servers.shutdown()
        log.logger().info('Server stopped.')

        
if (__name__ == '__main__'):
    server =  Server()
    server.start()
