#31.10.2010
#13.11.2010

from shared.network import ConnServer
import shared.man_db_msg_pb2 as proto
import time

class Master():
    def __init__(self, hw):
        self.hw = hw
        self.hw_count = 0

    def print_db(self, msg):
        print('HELLO:<'+self.hw+'> number:<'+str(self.hw_count)+'>')
        self.hw_count += 1
        print('msg.message_type', msg.message_type)
        print('msg.db:')
        
        print('  1.node:       --id--ip--')
        for record in msg.db.node:
            print('    node:', record.id, record.ip)
        
        print('  2.controller:  --id--address--node_id--')
        for record in msg.db.controller:
            print('    controller:', record.id, record.address, record.node_id)
        
        print('  3.point:      --id--type--controller_id--reader--')
        for record in msg.db.point:
            print('    point:', record.id, record.type, record.controller_id, record.reader)
        
        print('  4.user_group: --id--name--')
        for record in msg.db.user_group:
            print('    user_group:', record.id, record.name)
        
        print('  5.user:       --id--name--surname--patronymic--gender--')
        for record in msg.db.user:
            print('    user:', record.id, record.name, record.surname, record.patronymic, record.gender)
        
        print('  6.em_marine_card:')
    #    for record in msg.db.em_marine_card:
    #        print('    em_marine_card:', record.id, record.id)
        print('  7.point_group:')
    #    for record in msg.db.point_group:
    #        print('    point_group:', record.id, record.id)
        print('  8.script:')
    #    for record in msg.db.script:
    #        print('    script:', record.id, record.id)
        print('  9.access:')
    #    for record in msg.db.access:
    #        print('    access:', record.id, record.id)
        print('  10.point_group_link:')
    #    for record in msg.db.point_group_link:
    #        print('    point_group_link:', record.id, record.id)
        print('  11.user_group_link:')
    #    for record in msg.db.user_group_link:
    #        print('    user_group_link:', record.id, record.id)
        print('  12.log:')
    #    for record in msg.db.log:
    #        print('    log:', record.id, record.id)
        print('  13.database_send_log:')
    #    for record in msg.db.database_send_log:
    #        print('    database_send_log:', record.id, record.id)


if __name__ == '__main__':
    try:
        master_object = Master('HeLlO-wOrLd')
        net = ConnServer(("localhost", 10000), {'db_to_print': master_object.print_db})
        net.start_servers()
        time.sleep(10)
        net.shutdown()
        
    except Exception as exc:
        print('ERR:', repr(exc))
        net.shutdown()
