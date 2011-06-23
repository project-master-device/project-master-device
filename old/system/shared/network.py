# 30.10.10 - 
# 17.11.2010

#TODO: licence - MIT

import SocketServer #lowercase in python3 - TODO: use try
import socket
import sys
import threading
import time
from shared import log, config

import man_db_msg_pb2 as proto

head_delim = '@' #trashhh

#TODO: rename to IncomingTCPserver / OutcomingTCPserver / TCPserver
#TODO: delete TCPserver.start_servers / TCPserver.shutdown

# class to handle incoming '.proto as text' messages
class IncomingConnServer(SocketServer.TCPServer, threading.Thread, object):
    class RequestHandler(SocketServer.BaseRequestHandler):
        def handle(self):
            try:
                head = ''
                body = ''
                body_remain = 0
                while True:
                    head_char = self.request.recv(1)
                    if (head_char == head_delim):
                        break
                    else:
                        head += head_char
                body_remain = int(head)
                while body_remain > 0:
                    #head is a string int!!!!11
                    buff = self.request.recv(body_remain)
                    body_remain -= len(buff)
                    body += buff
                message = proto.man_message()
                message.ParseFromString(body)
            except Exception as exc:
                log.logger().error('IncomingTCPserver: error while reading message: ' + repr(exc))
            handler = self.server.handlers_dict[message.message_type]
            handler(message)

    def __init__(self, local_host_port, handlers_dict_):
        super(IncomingConnServer, self).__init__(local_host_port, self.RequestHandler)
        self.handlers_dict = handlers_dict_
        threading.Thread.__init__(self)
        self.daemon = True
        
    def run(self):
        self.serve_forever()


# class to send text messages
class OutcomingConnServer():
    def __init__(self):
        self._socket_lock = threading.Lock()
    
    def send_message(self, remote_host_port, msg_body):
        self._socket_lock.acquire()
        try:
            sock = socket.socket()
            sock.connect(remote_host_port)
            #head is a string int!!!!11
            str_msg = str(len(msg_body)) + head_delim + msg_body
            sock.send(str_msg)
            sock.close()
        finally:
            self._socket_lock.release()

# two text servers in one
class ConnServer():
    def __init__(self, local_host_port, handlers_dict):
        self.server_in = IncomingConnServer(local_host_port, handlers_dict)
        self.server_out = OutcomingConnServer()
    
    def start_servers(self):
        self.server_in.start()
        log.logger().info('TCPserver: started')

    def shutdown(self):
        self.server_in.shutdown()
        log.logger().info('TCPserver: stopped')

