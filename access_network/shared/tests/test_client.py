#31.10.2010
#13.11.2010

from shared.network import ConnServer
import shared.man_db_msg_pb2 as proto
import time

def test_handler(msg):
    print('MESSAGE:<\n', msg, '\n>')

if __name__ == '__main__':
    try:
        net = ConnServer(("localhost", 10001), {'default': test_handler})
        net.start_servers()

        msg = proto.man_message()
        msg.message_type = 'db_to_print'
        db_node1 = msg.db.node.add()
        db_node1.id = 1
        db_node1.ip = '120.45.210.10'
        db_node2 = msg.db.node.add()
        db_node2.id = 2
        db_node2.ip = '120.45.210.11'
        db_node3 = msg.db.node.add()
        db_node3.id = 3
        db_node3.ip = '120.45.210.12'
        db_point1 = msg.db.point.add()
        db_point1.id = 1
        db_point1.type = 'type_1'
        db_point1.controller_id = 1
        db_point1.reader = 'Reader_1'

        net.server_out.send_message(("localhost", 10000), msg.SerializeToString())
        time.sleep(0.1)
        net.server_out.send_message(("localhost", 10000), msg.SerializeToString())
        time.sleep(0.1)
        net.shutdown()
        
    except Exception as exc:
        print('ERR:', repr(exc))
        net.shutdown()
