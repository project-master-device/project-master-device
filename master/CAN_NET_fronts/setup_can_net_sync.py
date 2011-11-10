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


from distutils.core import setup, Extension
#import os, sys
#sys.path.append('.')

can_net_sync_ext = Extension('can_net_sync', sources = ['can_net_sync_py.c', 'can_net_sync_wrapper.c', '../../common/net/can_net.c',  '../../common/net/can_net_middle.c',  '../../common/net/can_net_core.c',  '../../common/net/can_protocols.c', '../local_net_CAN/can_drv_lin.c', '../local_net_CAN/can_socket.c', '../../common/lib/list.c'], define_macros=[('CAN_NET_LINUX', None)])

setup (name = 'can_net_sync',
        version = '0.1',
        description = 'can net sync module',
        ext_modules = [can_net_sync_ext])

print "GREAT_SUCCESS"

