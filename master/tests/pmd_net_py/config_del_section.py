import can_net_sync as cn
import pmd_net_system as sys

section = ("config_section", 42, ["options"])

data = sys.config_w_section_del(section)
if(data[0] == 0):
    msg = ("msg_lvl2", ("msg_metadata", 123, 1, 1, 1), data[1])
    print msg

cn.init(1, 100, 100, 100, 100)
cn.send(msg)
