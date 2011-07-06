# created 06.07.2011

from distutils.core import setup, Extension

pmd_net_led_ext = Extension('pmd_net_led', sources = ['led_py.c', 'led.c'])

setup (name = 'pmd_net_led',
        version = '0.1',
        description = 'pmd-net led module',
        ext_modules = [pmd_net_led_ext])

