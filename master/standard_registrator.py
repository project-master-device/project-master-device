# Copyright (c) 2010-2011 Gennady Kalashnikov
#
# This file is part of Project Master Device.
#
# Project Master Device is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Project Master Device is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.


import pmd_net_button as button
import pmd_net_system as sys
import pmd_net_led as led
import pmd_net_reader as reader
import can_net_sync as lnet

#v0.4
class StandardRegistrator:
	rc_ok = 0
	rc_processed = 1
	rc_net_error = 2	# has addition
	rc_packer_err = 3	# has addition
	rc_snd_err = 4		# has addition

	#registration states
	reg_states = {}
	reg_state_req_sent = 1

	# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)
	def try_register(msg, own_port, reg_slave_f, check_slave_reg_f):
		hwa = msg[1][1]
		
		#if NORMAL msg:
		if ( (msg[1][3] == 0) | ((msg[1][4] != sys.Ids['HEARTBEAT']) & (msg[1][4] != sys.Ids['CONFIG'])) ):
			return (rc_ok)
		
		#if CONFIG msg:
		if (msg[1][4] == sys.Ids['CONFIG']):
			if (reg_states.get(hwa) == reg_state_req_sent):
				#send command to set slave regime to Normal
				packer_rc, data = sys.set_op_w_normal()
				if (packer_rc != 0):
					return (rc_packer_err, packer_rc)
				msg_setop = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['SET_OP']), data)
				snd_rc = lnet.send(msg_setop)
				if (snd_rc != 0):
					return (rc_snd_err, snd_rc)
				
				reg_slave_f(msg)
				del reg_states[hwa]
				
				return (rc_processed)
			#else: unexpected config - panic
		
		#if HEARTBEAT msg:
		#else if already registered - quit:  (this way allows app to delete slaves without asking receive_message)
		if (check_slave_reg_f(hwa)):
			return (rc_processed)
		
		#send request for config (if it hvent been done already):
		if (reg_states.get(hwa) != reg_state_req_sent):
			packer_rc, data = sys.config_w_request()
			if (packer_rc != 0):
				return (rc_packer_err, packer_rc)
			msg_req = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['CONFIG']), data)
			snd_rc = lnet.send(msg_req)
			if (snd_rc != 0):
				return (rc_snd_err, snd_rc)
			else:
				#quit - only one msg handling on one registration_handler call
				reg_states[hwa] = reg_state_req_sent
				return (rc_ok)


"""
class MessageReciever:
	rc_ok = 0
	rc_rcv_error = 1
	rc_net_error = 2
	rc_packer_err = 3
	rc_snd_err = 4

	broadcast_port = 14 #to be fixed

	#registration states
	reg_states = {}
	reg_state_req_sent = 1

	# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)

	#TODO: ??? 2 operations:	rcv_broadcast(own_port, handle_msg_f, check_slave_reg_f)
	#							rcv_onport(own_port, handle_msg_f, reg_slave_f)

	#exits after receiving any msg or timeout/error
	#v0.3
	def recieve(own_port, handle_msg_f, reg_slave_f, check_slave_reg_f):
		#check broadcast port:
		rcv_rc, msg = lnet.recv(broadcast_port)
		if (rcv_rc != 0):
			#check own_port:
			rcv_rc, msg_p = lnet.recv(own_port)
			if (rcv_rc != 0):
				return (rc_rcv_error, rcv_rc)
			hwa = msg_p[1][0]
		else:
			hwa = msg[1][0]
		
		#if NORMAL msg:
		if ( (msg[1][3] == 0) | ((msg[1][4] != sys.Ids['HEARTBEAT']) & (msg[1][4] != sys.Ids['CONFIG'])) ):
			f_rc = handle_msg_f(msg)
			return (rc_ok, f_rc)
		
		#if CONFIG msg:
		if (msg[1][4] == sys.Ids['CONFIG']):
			if (reg_states.get(hwa) == reg_state_req_sent):
				#send command to set slave regime to Normal
				packer_rc, data = sys.set_op_w_normal()
				if (packer_rc != 0):
					return (rc_packer_err, packer_rc)
				msg_setop = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['SET_OP']), data)
				snd_rc = lnet.send(msg_setop)
				if (snd_rc != 0):
					return (rc_snd_err, snd_rc)
				
				reg_slave_f(msg[1])
				del reg_states[hwa]
				
				return (rc_ok, NULL)
			#else: unexpected config - panic
		
		#if HEARTBEAT msg:
		#else if already registered - quit: - to allow app to delete slaves without asking receive_message
		if (check_slave_reg_f(msg[1])):
			return (rc_ok, NULL)
		
		#send request for config (if it hvent been done already):
		if (reg_states.get(hwa) != reg_state_req_sent):
			packer_rc, data = sys.config_w_request()
			if (packer_rc != 0):
				return (rc_packer_err, packer_rc)
			msg_req = ("msg_lvl2", ("msg_metadata", hwa, own_port, 1, sys.Ids['CONFIG']), data)
			snd_rc = lnet.send(msg_req)
			if (snd_rc != 0):
				return (rc_snd_err, snd_rc)
			else:
				#quit - only one msg handling on one registration_handler call
				reg_states[hwa] = reg_state_req_sent
				return (rc_ok, NULL)

"""
