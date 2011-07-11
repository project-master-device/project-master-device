import can_net_sync as cn
import pmd_net_system as sys

section = ("config_section", 2, ["options", ("option", "type", "led", "s"), ("option", "test", "trololol", "s"), ("option", "ddr", 36, "I"), ("option", "port", 37, "I"), ("option", "offset", 4, "I")])
config = ["config_cnf", section]

print sys.config_w_section_add(section)
#msg = ("msg_lvl2", ("msg_metadata", 1, 1, 1, sys.config_w_section_add(config)[1]))
#print(msg)
#cn.init(1, 100, 100, 100, 100)
#cn.send(msg)
