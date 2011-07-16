import can_net_sync as cn
import pmd_net_led as l

cn.init(1, 100, 100, 100, 100)
msg = ("msg_lvl2", ("msg_metadata", 123, 1, 0, 1), l.w_toggle()[1])
cn.send(msg)
