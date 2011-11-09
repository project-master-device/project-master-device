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