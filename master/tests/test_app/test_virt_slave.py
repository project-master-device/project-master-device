#virtual test:

import time
#import random

import can_net_sync as lnet
import pmd_net_system as sys
import pmd_net_button as button
import pmd_net_led as led
import pmd_net_reader as reader

#from master import standard_registrator as reg
import standard_registrator as reg
import tests.pmd_net_py.config_full as test_cfg
import tests.common as common

current_slave_state = 0

def slave_msg_handler(msg):
		#if command msg: =normal msg
		#currently not used

		#if SETOP msg:
		if ( (msg[1][3] == 1) & (msg[1][4] != sys.Ids['SETOP']) ):
			print "slave received SETOP msg:"
			print msg
			#current_slave_state = int(msg[2])
			return 0
		
		#if CONFIG msg:
		if ( (msg[1][3] == 1) & (msg[1][4] != sys.Ids['CONFIG']) ):
			print "slave received CONFIG msg:"
			print msg
			#TODO: do smth
			return 0
				

def virt_slave():
	port = 3
	rci = can_net_sync_init(comm.port, comm.sync_msgs_limit, comm.sync_timeout, comm.send_frame_timeout_us, comm.net_confirm_timeout);
	if (rci):
		printf "initialization return code:", rci
		return 0;
	else:
		print "successful initialization\n"
	
	base_period = 0.1 # 100ms
	hwa = 13
	not_sys = 0
	is_sys = 1

	#button_id = random.randrange(0, 4)			
	button_id = 5
	rcp, btn_down_data = button.w_down()
	msg_btn_down = ("msg_lvl2", ("msg_metadata", hwa, comm.br_port, not_sys, button_id), btn_down_data)
	rcp, btn_up_data = button.w_up()
	msg_btn_up = ("msg_lvl2", ("msg_metadata", hwa, comm.br_port, not_sys, button_id), btn_up_data)

	no_id = 0
	rcp, hb_msg_data = sys.heartbeat_w()
	hb_msg = ("msg_lvl2", ("msg_metadata", hwa, comm.br_port, is_sys, no_id), hb_msg_data)
	
	x = 0
	while(True):
		msg = comm.recv_msg(port)
		if (msg[1][1] != hwa):
			print "Slave:", hwa, "recieved msg for slave:", msg[1][1]
		else:
			slave_msg_handler(msg)
		
		x += 1
		if (x == 9):
			snd_rc = lnet.send(msg_btn_down)
		if (x == 10):
			x = 0
			snd_rc = lnet.send(msg_btn_up)
		
		snd_rc = lnet.send(hb_msg)
		time.sleep(base_period)
	
if __name__ == "__main__":
	print "virtual slave - main"
