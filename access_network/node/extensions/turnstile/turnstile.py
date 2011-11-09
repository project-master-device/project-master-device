# x3.10.2010
# 17.12.2010

#TODO: licence - MIT

import datetime
import os
import threading
import time
import traceback
import struct

import shared.database as db
from node.points_manager import AccessPoint
from node.exceptions import *
from shared.config import Config
from shared import log
import node.local_protocols as lprotocols

class turnstile(AccessPoint):
    def __init__(self, parameters, local_net_send):
        self.__parameters = parameters
        self._local_net_send = local_net_send
        #TODO:FIX_DB_STRUCT decision and code_type AS strings
        self._key_type = 'em_marine_card'
        self._is_busy = False

        # BD script writer should know this:
        self.script_helpers = {None: self._deny, 'deny': self._deny, 'open_lim_time': self._open_lim_time, 'open': self._open, 'close':self._close}
        
        self._info = 'AccessPoint-turnstile id={0}'.format(self.__parameters['id'])

    ##### <handlers>
    
    def _close(self, key, hw_addr, device_id, args=None):
        self._send_signal(hw_addr, device_id, 0)
        #TODO:FIX_DB_STRUCT: decision='close'
        self._log('deny', key) 
    
    def _deny(self, key, hw_addr=None, device_id=None, args=None):
        self._log('deny', key)
    
    def _open_lim_time(self, key, hw_addr, device_id, args):
        '''
        @param args: delay (seconds) before closing the turnstile in this direction
        '''
        self._send_signal(hw_addr, device_id, 1)
        duration = args
        time.sleep(duration)
        self._close(key, hw_addr, device_id)
        self._log('allow_lim_time', key)
    
    def _open(self, key, hw_addr, device_id, args=None):
        self._send_signal(hw_addr, device_id, 1)
        #TODO:FIX_DB_STRUCT: decision='open'
        self._log('allow_lim_time', key)
    #### </handlers>
    
    #extention config: ok, this is hardcode, but where should we keep path to config for each extension?
    __config = Config(os.path.dirname(os.path.abspath(__file__)) + '/config.yml')
    
    def _log(self, decision, key):
        db.Log(time = datetime.datetime.now(), decision = 'allow_lim_time', \
                    point = self.__parameters['id'], code = key, code_type = self._key_type)
    
    @staticmethod
    def config():
        return turnstile.__config
    
    def _send_signal(self, hw_addr, device_id, signal):
        # `one uint32`-signal lvl3 protocol:
        msg_lvl2 = lprotocols.out_lvl2_common.pack_msg(hw_addr, device_id, 1, signal)
        serv_inf_int, data_int = lprotocols.out_lvl1.pack_msg(device_id, lprotocols.out_lvl2_common.version, msg_lvl2[0])
        data_str = struct.pack('Q', data_int) # v0.2 lprotocols use only 64bit msgs
        response_code = self._local_net_send((hw_addr, device_id), (serv_inf_int, data_str))
        if (response_code == None):
            log.logger().info(self._info + ': target device ({0}, {1}) did not respond'.format(hw_addr, device_id))
    
    # Concatenates controller address and port number.
    # There are many addresses of the same point object as it have many ports.
    # Returns list with all addresses.
    def address_list(self):
        res = db.Controller.select(db.AND(db.Controller.q.id == db.Point.q.controller, db.Point.q.id == self.__parameters['id']))
        
        if res.count() == 0:
            raise DbError("no 'controller' record in database for the 'point' with id = %d" % self.__parameters['id'])
        
        c_addr = res[0].address
        addr = [ (c_addr, self.__parameters['devices']['in']['reader']), (c_addr, self.__parameters['devices']['out']['reader']) ]
        return addr
    
    def process_msg(self, msg, hw_addr_recieved, device_id_recieved):
        '''
         - checks recieved key
         - sends respose to device when and if needed (in handler)
         - writes decision to log
        '''
        try:
            if (self._is_busy):
                log.logger().info(self._info + ' REFUSED a request from ({0}, {1}) ; it is busy'.format(hw_addr_recieved, device_id_recieved))
                return
            self._is_busy = True
            
            log.logger().debug(self._info + ': procesing msg from ({0}, {1})'.format(hw_addr_recieved, device_id_recieved))
            key = lprotocols.in_lvl3_32ints.unpack(msg)[0]
            
            decision_str = 'deny'
            hw_addr_sending = hw_addr_recieved
            device_id_sending = None
            if (device_id_recieved == self.__parameters['devices']['out']['reader']):
                direction =  '_out'
                device_id_sending = self.__parameters['devices']['out']['lock']
            elif (device_id_recieved == self.__parameters['devices']['in']['reader']):
                direction =  '_in'
                device_id_sending = self.__parameters['devices']['in']['lock']
            else:
                raise Exception('unknown device_id')
            
            #TODO: make logger thread: connect it on creation, save log through it
            db.initialize() #connect this thread to db
            helper_name, handler_args = self._AccessPoint__check_access(key, self.__parameters['id'])
            # key sending is a kostyil - create new, but bounded thread:
            decision_str = self.script_helpers[helper_name](key, hw_addr_sending, device_id_sending, handler_args)
            
            self._is_busy = False
        except Exception as exc:
            log.logger().error(self._info + ': error while processing msg from ({0}, {1}): '\
                                    .format(hw_addr_recieved, device_id_recieved) + '\n\t' + traceback.format_exc())

