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


import sys
# It is no more needed if set PYTHONPATH to the root of project
sys.path.append('../../../shared')
import config

conf = config.Config('../config/config.yml')

main = conf.getObject()
print 'Parameters of main:\n\t', [key + ' => ' + str(getattr(main, key)) for key in main.__dict__.keys()], '\n'

obj1 = conf.getObject('obj1')
print 'Parameters of obj1:\n\t', [key + ' => ' + str(getattr(obj1, key)) for key in obj1.__dict__.keys()], '\n'

obj2 = conf.getObject('obj2')
print 'Parameters of obj2:\n\t', [key + ' => ' + str(getattr(obj2, key)) for key in obj2.__dict__.keys()]
