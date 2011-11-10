# Copyright (c) 2010-2011 Sergey Gnuskov
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


import cmd
import can_net_sync
import pmd_net_led

class Console(cmd.Cmd):
    '''
    Interactive console class
    '''
    prompt = "pmd_system>"
    
    can_net_module = None
    cur_hw_addr = None
    cur_port = None
    
    def args_error(self, command):
        print "Argument error. Type \"help %s\" for more information" % command
    
    def do_init_can(self, line):
        '''
        Initialize CAN net.
        @ 1) port: port - uint8_t
        @ 2) msgs_limit: msgs limit - int
        @ 3) timeout_cycles: timeout cycles - int
        @ 4) send_frame_timeout_us: look at can_net::can_net_init() - const uint32_t
        @ 5) confirmation_tics: look at can_net::can_net_init() - const uint32_t 
        '''
        args = line.split(" ")
        if len(args) < 5: 
            self.args_error("init_can")
            return
        rc = can_net_sync.init(int(args[0]), int(args[1]), int(args[2]), int(args[3]), int(args[4])) 
        if rc != 0:
            print "Can_net initialization failed with return code %i" % rc
        else:
            self.cur_port = int(args[0])
            print "Successfully initialized!" 

    def exit(self):
        #some cleaning
        print "Goodbye!"

    def do_exit(self, line):
        '''
        Exit from console.
        '''
        self.exit()
        return True
    
    def do_EOF(self, line):
        '''
        Exit by Ctrl-D.
        '''
        print
        self.exit()
        return True
    
    def do_choose_uc(self, line):
        '''
        Choose uc to control.
        @ 1) hw_addr of controller
        '''
        if line == "":
            self.args_error("choose_uc")
            return
        self.cur_hw_addr = int(line)
        
    def do_status(self, line):
        '''
        Show status.
        '''
        print "Now controlling uc with hw_addr = " + repr(self.cur_hw_addr)
    
    def do_led(self, line):
        '''
        Control led.
        @ 1) led id;
        @ 2) command (on, off, toggle)
         '''
        args = line.split(" ")
        if len(args) < 2:
            self.args_error("led")
            return
        if args[1] not in {"on", "off", "toggle"}:
            self.args_error("led")
            return
        r = {"on": pmd_net_led.w_on,
             "off": pmd_net_led.w_off,
             "toggle": pmd_net_led.w_toggle}[args[1]]()
        if r[0] != 0:
            print "Fail with led.w_xxx, r[0] = " + repr(r[0])
            return
        msg = ("msg_lvl2", ("msg_metadata", self.cur_hw_addr, self.cur_port, 0, int(args[0])), r[1])
        rc = can_net_sync.send(msg)
        if rc != 0:
            print "Sending faild with return code %i" % rc
            
    
