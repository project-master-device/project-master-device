#import sys
#sys.path.append('/home/USERNAME/git/pmd/master/test')

from Console import Console
#from master.tests.test_app import testapp, test_virt_slave as virt_slave
import tests.test_app.testapp, tests.test_app.test_virt_slave as virt_slave

if __name__ == '__main__':
    try:
        #Console().cmdloop("Hello!\nThis is PMD interactive console. Control your devices easily!")
        print 'abcd'
    except Exception as exc:
        print exc
