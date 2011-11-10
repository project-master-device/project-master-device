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


import os, datetime, time
import sqlobject
from sqlobject.sqlbuilder import *
import threading

from shared import date_time
from shared import man_db_msg_pb2
from shared.log import logger
from shared.orm import *

# Class for doing complex database operations
# and manage database connections in one thread.
# Again: one Manager per one thread.
class Manager:
    def __init__(self, connection_string, debug_mode = False):
        # Remove old cached connection
        sqlobject.dbconnection.TheURIOpener.cachedURIs.pop(connection_string, None)
        # Create new connection
        sqlobject.sqlhub.threadConnection = sqlobject.connectionForURI(connection_string)
        sqlobject.sqlhub.threadConnection.debug = debug_mode
        
        self.__drop_relations = [Access, Script, PointGroupLink, UserGroupLink, PointGroup, UserGroup, EmMarineCard, User, Point, Controller, Node]

    # with/as protocol for using transactions
    def __enter__(self):
        logger().debug('Start transaction')

        # Create new transaction connection and swap with the current connection
        self.__connection_old = sqlobject.sqlhub.threadConnection
        transaction_connection = sqlobject.sqlhub.threadConnection.transaction()
        sqlobject.sqlhub.threadConnection = transaction_connection

        return sqlobject.sqlhub.threadConnection
    
    def __exit__(self, exc_type, exc_value, exc_info):
        if exc_type is None:
            sqlobject.sqlhub.threadConnection.commit()
            sqlobject.sqlhub.threadConnection = self.__connection_old
            self.__connection_old = None

            logger().debug('Transaction success')
        else:
            sqlobject.sqlhub.threadConnection.rollback()
            sqlobject.sqlhub.threadConnection = self.__connection_old
            self.__connection_old = None

            logger().error('Transaction failed')

            raise exc_type, exc_value
        pass

    # Return transaction object 
    def transaction(self):
        return self

    # Return connection object
    def connection(self):
        return sqlobject.sqlhub.threadConnection

    # Database specific SQL query to reset id counter.
    #def reset_id_counter(self, relation):
    #   conn.queryOne('DELETE FROM SQLITE_SEQUENCE WHERE name=%s' % relation.sqlmeta.table)

    # Drop all rows from all database relations in one transaction
    def drop_sync_data(self):
        logger().info('dropping sync data from database')

        try:
            for relation in self.__drop_relations:
                self.connection().query(self.connection().sqlrepr(Delete(relation.sqlmeta.table, None)))

        except Exception:
            logger().error('failed to drop sync data from database')
            raise

    # Drop all rows from Log database relation.
    def drop_log(self):
        logger().info('dropping log from database')

        try:
            self.connection().query(self.connection().sqlrepr(Delete(Log.sqlmeta.table, None)))

        except Exception:
            logger().error('failed to drop log from database')
            raise

    # Parameter 'msg' should be a protobuf FullMANDB message. 
    def load_sync_data(self, msg = None, **kwargs):
        logger().info('loading sync data to database from proto message')

        if msg is None:
            if kwargs.has_key('msg'):
                msg = kwargs['msg']

        try:
            # DatabaseSendLog and Log relations are not participate in full synchronization
                
            for node in msg.node:
                Node(id = node.id, ip = (node.HasField('ip') and node.ip or None))
    
            for controller in msg.controller:
                Controller(id = controller.id, address = controller.address, node = controller.node_id)
    
            for point in msg.point:
                Point(id = point.id, type = point.type, controller = point.controller_id, reader = (point.HasField('reader') and point.reader or None))
    
            for point_group in msg.point_group:
                PointGroup(id = point_group.id, name = point_group.name)
    
            for point_group_link in msg.point_group_link:
                PointGroupLink(id = point_group_link.id, point = point_group_link.point_id, point_group = point_group_link.point_group_id)
    
            for user in msg.user:
                User(id = user.id, name = (user.HasField('name') and user.name or None), surname = (user.HasField('surname') and user.surname or None), patronymic = (user.HasField('patronymic') and user.patronymic or None), gender = (user.HasField('gender') and user.gender or None))
    
            for em_marine_card in msg.em_marine_card:
                EmMarineCard(id = em_marine_card.id, code = em_marine_card.code, user = em_marine_card.user_id)
    
            for user_group in msg.user_group:
                UserGroup(id = user_group.id, name = user_group.name)
    
            for user_group_link in msg.user_group_link:
                UserGroupLink(id = user_group_link.id, user = user_group_link.user_id, user_group = user_group_link.user_group_id)
    
            for script in msg.script:
                Script(id = script.id, condition_code = script.condition_code)
    
            for access in msg.access:
                Access(id = access.id, script = access.script_id, point_group = access.point_group_id, user_group = access.user_group_id)

        except Exception:
            logger().error('failed to load sync data to database from proto message')
            raise

    # Parameter 'msg' should be a protobuf FullMANDB message.
    # Necessary only log part of the message. 
    def load_log(self, msg = None, **kwargs):
        logger().info('loading log to database from proto message')

        if msg is None:
            if kwargs.has_key('msg'):
                msg = kwargs['msg']

        try:
            for log in msg.log:
                Log(time = date_time.datetime_from_string(log.time), decision = log.decision, point = log.point_id, code = (log.HasField('code') and log.code or None), code_type = (log.HasField('code_type') and log.code_type or None))

        except Exception:
            logger().error('failed to load log to database from proto message') 
            raise

    # Returns FullMANDB message.
    # 'msg' parameter -- FullMANDB message to fill with sync data. If not given -- creates new.
    def get_sync_data(self, msg = None, select_for_update = False, **kwargs):
        logger().info('getting sync data from database to proto message')

        if msg is None:
            if kwargs.has_key('msg'):
                msg = kwargs['msg']
            else:
                msg = man_db_msg_pb2.man_message.FullMANDB()

        if select_for_update is None:
            if kwargs.has_key('select_for_update'):
                select_for_update = kwargs['select_for_update']
 
        try:
            nodes = Node.select(forUpdate = select_for_update)
            for node in nodes:
                node_msg = msg.node.add()

                node_msg.id = node.id
                if node.ip: node_msg.ip = node.ip

            controllers = Controller.select(forUpdate = select_for_update)
            for controller in controllers:
                controller_msg = msg.controller.add()

                controller_msg.id = controller.id
                controller_msg.address = controller.address
                controller_msg.node_id = controller.node.id

            points = Point.select(forUpdate = select_for_update)
            for point in points:
                point_msg = msg.point.add()

                point_msg.id = point.id
                point_msg.type = point.type
                point_msg.controller_id = point.controller.id
                if point.reader: point_msg.reader = point.reader

            point_groups = PointGroup.select(forUpdate = select_for_update)
            for point_group in point_groups:
                point_group_msg = msg.point_group.add()

                point_group_msg.id = point_group.id
                point_group_msg.name = point_group.name

            point_group_links = PointGroupLink.select(forUpdate = select_for_update)
            for point_group_link in point_group_links:
                point_group_link_msg = msg.point_group_link.add()
                point_group_link_msg.id = point_group_link.id
                point_group_link_msg.point_id = point_group_link.point.id
                point_group_link_msg.point_group_id = point_group_link.point_group.id

            users = User.select(forUpdate = select_for_update)
            for user in users:
                user_msg = msg.user.add()

                user_msg.id = user.id
                if user.name: user_msg.name = user.name
                if user.surname: user_msg.surname = user.surname
                if user.patronymic: user_msg.patronymic = user.patronymic
                if user.gender: user_msg.gender = user.gender

            em_marine_cards = EmMarineCard.select(forUpdate = select_for_update)
            for em_marine_card in em_marine_cards:
                em_marine_card_msg = msg.em_marine_card.add()

                em_marine_card_msg.id = em_marine_card.id
                em_marine_card_msg.code = em_marine_card.code
                em_marine_card_msg.user_id = em_marine_card.user.id

            user_groups = UserGroup.select(forUpdate = select_for_update)
            for user_group in user_groups:
                user_group_msg = msg.user_group.add()

                user_group_msg.id = user_group.id
                user_group_msg.name = user_group.name

            user_group_links = UserGroupLink.select(forUpdate = select_for_update)
            for user_group_link in user_group_links:
                user_group_link_msg = msg.user_group_link.add()

                user_group_link_msg.id = user_group_link.id
                user_group_link_msg.user_id = user_group_link.user.id
                user_group_link_msg.user_group_id = user_group_link.user_group.id

            scripts = Script.select(forUpdate = select_for_update)
            for script in scripts:
                script_msg = msg.script.add()

                script_msg.id = script.id
                script_msg.condition_code = script.condition_code

            accesses = Access.select(forUpdate = select_for_update)
            for access in accesses:
                access_msg = msg.access.add()

                access_msg.id = access.id
                access_msg.script_id = access.script.id
                access_msg.point_group_id = access.point_group.id
                access_msg.user_group_id = access.user_group.id

            return msg

        except Exception:
            logger().error('failed to get sync data from database to proto message')
            raise

    # Set log part of the FullMANDB message and returns it.
    # 'msg' parameter -- FullMANDB message to fill with Log data. If not given -- creates new.
    def get_log(self, msg = None, select_for_update = False, **kwargs):
        logger().info('getting log from database to proto message')

        if msg is None:
            if kwargs.has_key('msg'):
                msg = kwargs['msg']
            else:
                msg = man_db_msg_pb2.man_message.FullMANDB()

        if select_for_update is None:
            if kwargs.has_key('select_for_update'):
                select_for_update = kwargs['select_for_update']
 
        try:
            logs = Log.select()
            for log in logs:
                log_msg = msg.log.add()

                log_msg.time = str(log.time)
                log_msg.decision = log.decision
                log_msg.point_id = log.point.id
                if log.code: log_msg.code = log.code
                if log.code_type: log_msg.code_type = log.code_type

            return msg

        except Exception:
            logger().error('failed to get log data from database to proto message')
            raise

__mutex = threading.Lock()
__managers = {}
__connection_string = None
__debug_mode = False

def initialize(connection_string = None, debug_mode = None):
    global __debug_mode
    global __connection_string

    with __mutex:
        if connection_string is not None: __connection_string = connection_string
        if debug_mode is not None: __debug_mode = debug_mode

        __managers[threading.current_thread()] = Manager(__connection_string, __debug_mode)

def cleaner():
    while True:
        for thr in __managers.keys():
            with __mutex:
                if not thr.isAlive():
                    __managers.pop(thr, None)

        time.sleep(60)

def manager():
    thr = threading.current_thread()

    if not __managers.has_key(thr):
        with __mutex:
            __managers[thr] = Manager(__connection_string, __debug_mode)

    return __managers[thr]

__cleaner_thread = threading.Thread(target = cleaner)
__cleaner_thread.daemon = True
__cleaner_thread.start()

