import can_net_sync as cn
import pmd_net_system as sys

section1 = ("config_section", 42, ["options", ("option", "type", "led", sys.ConfigOptionTypes['STRING']), ("option", "qwer", 123, sys.ConfigOptionTypes['UINT']), ("option", "rewq", -333, sys.ConfigOptionTypes['INT'])])

section2 = ("config_section", 24, ["options", ("option", "option", 321, sys.ConfigOptionTypes['UINT']), ("option", "avavavavafaf", -666, sys.ConfigOptionTypes['INT'])])

config = ["config_cnf", section1, section2]

data = sys.config_w_full(config)
if(data[0] == 0):
    msg = ("msg_lvl2", ("msg_metadata", 123, 1, 1, sys.Ids['CONFIG']), data[1])
    print msg
    cn.init(1, 100, 100, 100, 100)
    cn.send(msg)
