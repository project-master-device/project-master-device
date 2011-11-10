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

pmd_net_system_ext = Extension('pmd_net_system', sources = ['system_py.c', 'config.c', '../../lib/config_core.c', '../../lib/list.c', 'heartbeat.c', 'set_op.c'])

setup (name = 'pmd_net_system',
        version = '0.1',
        description = 'pmd-net system module',
        ext_modules = [pmd_net_system_ext])

