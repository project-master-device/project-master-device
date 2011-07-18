import can_net_sync as cn
import pmd_net_system as sys

section = ("config_section", 42, ["options", ("option", "type", "led", sys.ConfigOptionTypes['STRING']), ("option", "qwer", 123, sys.ConfigOptionTypes['UINT']), ("option", "rewq", -333, sys.ConfigOptionTypes['INT'])])

data = sys.config_w_section_add(section)
print section
    
if(data[0] == 0):
    msg = ("msg_lvl2", ("msg_metadata", 123, 1, 1, sys.Ids['CONFIG']), data[1])
    cn.init(1, 100, 100, 100, 100)
    cn.send(msg)
