#created 08.07.2011

import pmd_net_button as button
import pmd_net_system as system
import pmd_net_led as led
import pmd_net_reader as reader
import can_net_sync as lnet

# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)

led_1_id = 1
led_2_id = 2
button_1_id = 3
button_2_id = 4
reader_1_id = 5
reader_2_id = 6

def test_write():
	lnet.init(1, 10, -1, 1000, 1000)
	lnet.send( ("msg_lvl2", ("msg_metadata", 15, 1, 0, 18), "STARTING_TEST") )
	
	hwa = 15
	port = 2
	n = 0
	s = 1
	rc, lvl3_msg = button.w_up()
	if (rc):
		print "fail button.w_up, rc = ", rc
		return
	else:
		print "sending button.w_up"
		msg = ("msg_lvl2", ("msg_metadata", hwa, port, n, button_1_id), lvl3_msg)
		lnet.send(msg)

	rc, lvl3_msg = button.w_down()
	if (rc):
		print "fail button.w_down, rc = ", rc
		return
	else:
		print "sending button.w_down"
		msg = ("msg_lvl2", ("msg_metadata", hwa, port, n, button_2_id), lvl3_msg)
		lnet.send(msg)


test_write()
