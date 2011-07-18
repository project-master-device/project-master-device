import can_net_sync as cn
import pmd_net_system as sys

cn.init(1, 100, 100, 100, 100)
msg = ("msg_lvl2", ("msg_metadata", 123, 1, 1, sys.Ids['SET_OP']), sys.set_op_w_init()[1])
print msg
cn.send(msg)
