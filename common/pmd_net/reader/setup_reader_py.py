# created 06.07.2011

from distutils.core import setup, Extension

pmd_net_reader_ext = Extension('pmd_net_reader', sources = ['reader_py.c', 'reader.c'])

setup (name = 'pmd_net_reader',
        version = '0.1',
        description = 'pmd-net reader module',
        ext_modules = [pmd_net_reader_ext])

