import logging
import logging.config
from threading import Lock

# Module-singleton for access to the main logger

__logger = None
__mutex = Lock()

# Reload logger configuration from the log_conf_path file
def initialize(log_conf_path):
    global __logger
    with __mutex:
        logging.config.fileConfig(log_conf_path)
        # FIXME 
        # Hack it, yeah!
        logging.Logger.manager.loggerDict.pop('root', None)
        __logger = logging.getLogger('root')

# Access to the __logger attribute
def logger():
    return __logger

