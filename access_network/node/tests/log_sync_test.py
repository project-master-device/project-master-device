# Copyright (c) 2010-2011 Timofey Kirillov
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


import node.main
from shared import config
from shared.network import OutcomingConnServer as ocs
from node.log_synchronizer import LogSynchronizer

if (__name__ == '__main__'):
	try:
		log = LogSynchronizer(ocs())
		log.run()
	except KeyboardInterrupt:
		print 'Stopped.'