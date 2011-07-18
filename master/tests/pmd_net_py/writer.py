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

def send_msg(hwa, port, descr_str, d_id, writer_func, smb = 0):
	rc, lvl3_msg = writer_func()
	if (rc != 0):
		print "fail ", descr_str, ", rc = ", rc
		return
	else:
		print "sending ", descr_str
		msg = ("msg_lvl2", ("msg_metadata", hwa, port, smb, d_id), lvl3_msg)
		lnet.send(msg)

def test_write():
	sync_timeout = 1500 # *1000us = 1.5s
	drv_send_timeout = 500000 # *1us = 0.5s
	net_confirm_timeout = 1000 # *1000us = 1s
	lnet.init(1, 10, sync_timeout, drv_send_timeout, net_confirm_timeout)
	lnet.send( ("msg_lvl2", ("msg_metadata", 15, 1, 0, 18), "STARTING_TEST") )
	
	hwa = 15
	port = 2
	s = 1
	send_msg(hwa, port, "button.w_up", button_1_id, button.w_up)
	send_msg(hwa, port, "button.w_down", button_2_id, button.w_down)
	send_msg(hwa, port, "led.w_on", led_1_id, led.w_on)
	send_msg(hwa, port, "led.w_off", led_2_id, led.w_off)
	send_msg(hwa, port, "led.w_toggle", led_2_id, led.w_toggle)

test_write()
