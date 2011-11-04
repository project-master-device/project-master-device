#virtual test:
#as it was 16.10.2011
#not srsly tested yet

import time

import can_net_sync as lnet
import pmd_net_system as sys
import pmd_net_button
import pmd_net_led
import pmd_net_reader

#from master import standard_registrator as reg
#import master.standard_registrator as reg
#from pmd.master import standard_registrator as reg
import standard_registrator as reg
import tests.common as comm

# picture of the world:
slaves = {}

#hwa - int
def check_slave_reg_f(hwa):
	if (slaves.get(hwa) == None):
		return False
	else:
		return True

# msg with config
def reg_slave_f(msg):
	slaves[msg[1][1]] = msg[2]

# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
def handle_msg(msg):
	print "from hwa=", msg[1][1], "port=", msg[1][2], "smb=", msg[1][3], "id=", msg[1][4], "\r\n DATA=", msg[2] 

def test_app():
	port = 3
	rci = can_net_sync_init(port, comm.sync_msgs_limit, comm.sync_timeout, comm.send_frame_timeout_us, comm.net_confirm_timeout);
	if (rci):
		printf "initialization return code:", rci
		return 0;
	else:
		print "successful initialization\n"
	
	registrator = reg.StandardRegistrator()
	
	base_period = 0.05 # 50ms
	while(True):
		msg = comm.recv_msg(port)
		if (msg != None):
			r = registrator.try_register(msg, port, reg_slave_f, check_slave_reg_f)
			if (r[0] == reg.rc_net_error):
				print "try_register:rc_net_error:", r[1]
			elif (r[0] == reg.rc_packer_err):
				print "try_register:rc_packer_err", r[1]
			elif (r[0] == reg.rc_snd_err):
				print "try_register:rc_snd_err", r[1]
			elif (r[0] == reg.rc_processed):
				print "try_register:rc_processed"
			elif (r[0] == reg.rc_ok):
				print "try_register:rc_OK"
				handle_msg(msg)
		
		time.sleep(base_period)
		
if __name__ == "__main__":
	print "test app - main"
