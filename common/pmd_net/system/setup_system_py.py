# created 06.07.2011

from distutils.core import setup, Extension

pmd_net_system_ext = Extension('pmd_net_system', sources = ['config_py.c', 'config.c', 'heartbeat_py.c', 'heartbeat.c', 'set_op_py.c', 'set_op.c'])

setup (name = 'pmd_net_system',
        version = '0.1',
        description = 'pmd-net system module',
        ext_modules = [pmd_net_system_ext])

