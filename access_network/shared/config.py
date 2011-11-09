import yaml
from threading import Lock

class Config:
    class Result: pass
    
    # Set path to YAML config file in constructor
    def __init__(self, path):
        file = open(path)
        self.__conf = yaml.load(file)
        file.close()
    
    # Get object 'obj' from config or whole file if nothing given
    def getObject(self, obj=None):
        res = Config.Result()
        if obj!= None:
            for param in self.__conf[obj]:
                setattr(res, param, self.__conf[obj][param])
        else:
            for param in self.conf:
                setattr(res, param, self.__conf[param])
        return res

    # Using config as dictionary to get parameters
    def __getitem__(self, key):
        return self.__conf[key]

# Attribute, which contains main config, after initializing
__config = None
__mutex = Lock()

# Bind __config attrubite to the given config file path
def initialize(conf_path):
    global __config
    with __mutex:
        __config = Config(conf_path)

# Access to the __config attribute
def config():
    return __config

