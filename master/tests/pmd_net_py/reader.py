#created 08.07.2011

import pmd_net_system as system
import pmd_net_button as button
import pmd_net_led as led
import pmd_net_reader as reader
import can_net_sync as lnet
import time

led_1_id = 1
led_2_id = 2
button_1_id = 3
button_2_id = 4
reader_1_id = 5
reader_2_id = 6

BUTTON_OP_DOWN = 1
BUTTON_OP_UP = 2
def button_handle(lvl2_msg_data, params):
	rc, op = button.read(lvl3_msg)
	s = params['state']
	if (op == None):
		print " button_handle: op=NONE"
		return None
	elif (op == BUTTON_OP_DOWN):
		s = True
	elif (op == BUTTON_OP_UP):
		s = False
	return 0

LED_OP_ON = 1
LED_OP_OFF = 2
LED_OP_TOGGLE = 3
def led_handle(lvl2_msg_data, params):
	rc, op = led.read(lvl3_msg)
	s = params['state']
	if (op == None):
		print " led_handle: op=NONE"
		return None
	elif (op == LED_OP_ON):
		s = True
	elif (op == LED_OP_OFF):
		s = False
	elif (op == LED_OP_TOGGLE):
		if (s == True):
			s = False
		else:
			s = True
	return 0

READER_OP_SEND_MSG = 0
READER_OP_GREEN_LED_ON = 1
READER_OP_GREEN_LED_OFF = 2
READER_OP_BEEP_ON = 3
READER_OP_BEEP_OFF = 4
def reader_handle(lvl2_msg_data, params):
	rc, op, code = reader.read(lvl3_msg)
	if (op == None):
		print " reader_handle: op=NONE"
		return None
	elif (op == READER_OP_SEND_MSG):
		if (code == None):
			print "code=None WHEN IT SHOULD NOT!"
		else:
			# do it here:
			print "\nCODE RECIEVED:", code
			return (0, code)
	elif (op == READER_OP_GREEN_LED_ON):
		params['led_state'] = True
	elif (op == READER_OP_GREEN_LED_OFF):
		params['led_state'] = False
	elif (op == READER_OP_BEEP_ON):
		params['beep_state'] = True
	elif (op == READER_OP_BEEP_OFF):
		params['beep_state'] = False
	return (0, None)

hwa = 15
controller_state = 0
devices = { led_1_id: {'name': 'led_1', 'state': False, 'id': led_1_id, 'handler': led_handle}, 
			led_2_id: {'name': 'led_2', 'state': False, 'id': led_2_id, 'handler': led_handle}, 
			button_1_id: {'name': 'button_1', 'state': False, 'id': button_1_id, 'handler': button_handle},
			button_2_id: {'name': 'button_2', 'state': False, 'id': button_2_id, 'handler': button_handle}, 
			reader_1_id: {'name': 'reader_1', 'led_state': False, 'beep_state': False, 'id': reader_1_id, 'handler': reader_handle},
			reader_2_id: {'name': 'reader_1', 'led_state': False, 'beep_state': False, 'id': reader_2_id, 'handler': reader_handle} }

# ("msg_lvl2", ("msg_metadata", hw_addr -int, port -int, is_system -int, id - int), data -string)

def handle_msg(msg):
	id = msg[1][4]
	device = devices[id]
	return device['handler'](msg[1][5], device)

def test_read():
	lnet.init(2, 10, -1, 1000, 1000)
	x = True
	while(x):
		rc, msg = lnet.recv(2)
		msg = ()
		if (rc != 0):
			print "lnet.recv(port) FAIL: rc= ", rc
		else:
			print "msg=", msg
			handle_msg(msg)
		time.sleep(0.01) #ms


test_read()
print
print "controller_state = ", controller_state
print devices
