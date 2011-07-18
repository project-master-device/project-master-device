import can_net_sync as cn
import pmd_net_system as sys

data = sys.config_w_section_del(1)

if(data[0] == 0):
    msg = ("msg_lvl2", ("msg_metadata", 123, 1, 1, sys.Ids['CONFIG']), data[1])
    print msg
    cn.init(1, 100, 100, 100, 100)
    cn.send(msg)
