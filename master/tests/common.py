
port = 3
sync_msgs_limit = 10
sync_timeout = 1500 # *1000us = 1.5s
send_frame_timeout_us = 500000 # *1us = 0.5s
net_confirm_timeout = 1000 # *1000us = 1s

br_port = broadcast_port = 14 #to be fixed

def recv_msg(own_port_):
	#check broadcast port:
	rcv_rc, msg = lnet.recv(broadcast_port)
	if (rcv_rc != 0):
		print "RECV ERR:", rcv_rc
		#check own_port:
		rcv_rc, msg_p = lnet.recv(own_port_)
		if (rcv_rc != 0):
			print "RECV ERR:", rcv_rc
			return None
		else:
			return msg_p
	else:
		return msg
