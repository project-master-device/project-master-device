# created 06.07.2011

from distutils.core import setup, Extension

pmd_net_button_ext = Extension('pmd_net_button', sources = ['button_py.c', 'button.c'])

setup (name = 'pmd_net_button',
        version = '0.1',
        description = 'pmd-net button module',
        ext_modules = [pmd_net_button_ext])

