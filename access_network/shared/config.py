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

