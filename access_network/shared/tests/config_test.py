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
