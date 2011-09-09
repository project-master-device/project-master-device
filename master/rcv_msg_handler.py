#created 08.09.2011

import pmd_net_button as button
import pmd_net_system as sys
import pmd_net_led as led
import pmd_net_reader as reader
import can_net_sync as lnet

rc_ok = 0
rc_rcv_error = 1
rc_net_error = 2
rc_packer_err = 3
rc_snd_err = 4

broadcast_port = 14 #tobe fixed

#, handle_sys_msg_f

# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)

#v0.1
handle_next_msg(own_port, handle_msg_f, reg_slave_f, check_slave_reg_f):
	#get next message:
	rcv_rc, msg = lnet.recv(broadcast_port)
	if (rcv_rc != 0):
		return (rc_rcv_error, rcv_rc)
	hwa = msg[1][0]
	
	#if msg is not heartbeat handle message normally:
	if ( (msg[1][3] != 1) | (msg[1][4] != sys.Ids['HEARTBEAT']) ):
		f_rc = handle_msg_f(msg)
		return (rc_ok, f_rc)

	if (check_slave_reg_f(msg[1])):
		return (rc_ok, NULL)

	#send request for config:
	packer_rc, data = sys.config_w_request()
	if (packer_rc != 0):
		return (rc_packer_err, packer_rc)
	msg_req = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['CONFIG']), data)
	snd_rc = lnet.send(msg_req)
	if (snd_rc != 0)
		return (rc_snd_err, snd_rc)

	#try to receive config:
	while(True):
		rcv_rc, msg_cfg = lnet.recv(own_port)
		if (rcv_rc == 0):
			#msg is from same slave?:
			elif (msg[1][0] != hwa):
				f_rc = handle_msg_f(msg_cfg)
				#f_rc is lost here
				#TODO: find way to deliver it to app 
				# ?save stage of registration process and always quit on after msg
				#set minimum timeout - to help network receive config
			elif ( (msg[1][3] != 1) | (msg[1][4] != sys.Ids['CONFIG']) ):
				#msg is not config:
				#PANIC - should never happen!
			else:
				#is config:
				break;
		#elif (rcv_rc != lnet.recv_rc_timeout):
			#!no errors in receive functions
			#return (rc_rcv_error, rcv_rc) #!
		else:
			#receive timeout - already waited enought (or did we?)
			#optional additional timeout
	
	#set slave regime to Normal
	packer_rc, data = sys.set_op_w_normal()
	if (packer_rc != 0):
		return (rc_packer_err, packer_rc)
	msg_setop = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['SET_OP']), data)
	snd_rc = lnet.send(msg_setop)
	if (snd_rc != 0)
		return (rc_snd_err, snd_rc)
	reg_slave_f(msg[1])
	return (rc_ok, NULL)


def test_write():
	sync_timeout = 1500 # *1000us = 1.5s
	drv_send_timeout = 500000 # *1us = 0.5s
	net_confirm_timeout = 1000 # *1000us = 1s
'''
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
'''
test_write()
