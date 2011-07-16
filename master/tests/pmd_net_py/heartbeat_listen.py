import can_net_sync as cn
import pmd_net_led as l
import time as t

cn.init(14, 100, 100, 100, 100)

msg = cn.recv(1)
while(1):
    t.sleep(1)
    msg = cn.recv(14)
    print msg
