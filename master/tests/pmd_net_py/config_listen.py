import can_net_sync as cn
import pmd_net_system as sys
import time as t

cn.init(1, 100, 100, 100, 100)

msg = cn.recv(1)
while(1):
    t.sleep(1)
    msg = cn.recv(1)
    if(msg[0] == 0):
        data = sys.config_read(msg[1][2])
        print data[2]
