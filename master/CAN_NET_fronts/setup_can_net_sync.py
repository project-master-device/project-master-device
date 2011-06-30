from distutils.core import setup, Extension
#import os, sys
#sys.path.append('.')

can_net_sync_ext = Extension('can_net_sync', sources = ['can_net_sync_py.c', 'can_net_sync_wrapper.c', '../../common/net/can_net.c',  '../../common/net/can_net_middle.c',  '../../common/net/can_net_core.c',  '../../common/net/can_protocols.c', '../local_net_CAN/can_drv_lin.c', '../local_net_CAN/can_socket.c', '../../common/lib/list.c'], define_macros=[('CAN_NET_LINUX', None)])

setup (name = 'can_net_sync',
        version = '0.1',
        description = 'can net sync module',
        ext_modules = [can_net_sync_ext])

print "GREAT_SUCCESS"

