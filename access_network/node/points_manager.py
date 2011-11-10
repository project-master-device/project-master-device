# Copyright (c) 2010-2011 Gennady Kalashnikov, Timofey Kirillov
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


import traceback
import thread

import shared.database as db
from shared.config import config
from shared import log
from node.exceptions import *

class AccessPoint:
    @staticmethod
    def config():
        assert 0

    def address_list(self):
        assert 0

    def process_msg(self, msg, recieved_device_addr):
        assert 0

    # Check access from database for the code and the point with point_id.
    @staticmethod
    def __check_access(code, point_id):
        scripts = db.Script.select(db.AND(db.Script.q.id == db.Access.q.script, db.Access.q.user_group == db.UserGroup.q.id, db.UserGroup.q.id == db.UserGroupLink.q.user_group, db.UserGroupLink.q.user == db.User.q.id, db.User.q.id == db.EmMarineCard.q.user, db.Access.q.point_group == db.PointGroup.q.id, db.PointGroup.q.id == db.PointGroupLink.q.point_group, db.PointGroupLink.q.point == db.Point.q.id, db.EmMarineCard.q.code == code, db.Point.q.id == point_id), distinct = True)
        
        #res format : (helper_name, additional_data)
        #res = (None, None)
        
        # Run each script in order.
        priority_max = -1
        priority = 0
        for script in scripts:
            log.logger().info('check_access: checking condition code')
            log.logger().debug('\n' + script.condition_code)
            
            try:
                # Execute condition_code definition in 'code_env' environment
                code_env = {}
                exec(script.condition_code, code_env)
                priority = code_env['self_priority']
                if (priority > priority_max):
                    priority_max = priority 
                    res = code_env['check']()
            except Exception, exc:
                raise Exception('failed to check condition code: ' + repr(exc))
        
        log.logger().info('check_access: result: ' + repr(res))
        return res


class PointsManager:
    def __init__(self, local_net_send):
        log.logger().info('initializing PointsManager')
        
        try:
            extenions = config()['extensions']
            
            self.points = {}
            
            for point_ext in extenions:
                log.logger().info('loading extension %s' % point_ext)
                
                try:
                    point_mod = __import__('node.extensions.' + point_ext + '.' + point_ext, fromlist = ['node', 'extensions', point_ext])
                    point_class = getattr(point_mod, point_ext)
                    
                    for point_parameters in point_class.config():
                        log.logger().info('creating access point')
                        
                        try:
                            point_object = point_class(point_parameters, local_net_send)
                            addresses = point_object.address_list()
                            for addr in addresses:
                                self.points[addr] = point_object
                        
                        except DbError, exc:
                            log.logger().error('failed to create access point: ' + traceback.format_exc())
                        
                except Exception, exc:
                    log.logger().error('failed to load extension: ' + traceback.format_exc())
        
        except Exception:
            log.logger().fatal('failed to initialize PointsManager')
            raise
    
    def process_msg(self, hw_addr, device_id, data):
        point = self.points[(hw_addr, device_id)]
        thread.start_new_thread(point.process_msg, (data, hw_addr, device_id))

#        point = self.points.get((hw_addr, device_id))
#        if (point != None):
#            point.process_msg(data, hw_addr, device_id)
#        else:
#            raise Exception('message from unknown device: hw_addr={0}, device_id={1}'.format(hw_addr, device_id))
