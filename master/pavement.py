from paver.easy import *
from paver.setuputils import setup
from distutils.core import Extension
import os, sys
sys.path.append('.')

import tools.sql_sparser as sql_sparser
import tools.db2proto_gen as db2proto_gen
import tools.db2orm_gen as db2orm_gen
import shared.config

can_socket_ext = Extension('can_socket', sources = ['node/can_socket/can_socket.c'])

setup (name = 'can_socket',
        version = '0.02',
        description = 'can socket module',
        ext_modules = [can_socket_ext])

@task
@consume_args
def gen_proto(args):
    '''Generate proto from sql file'''
    print '-'*50
    print 'Generating .proto\n'
    
    if len(args) < 2: 
        print '''Using 'config/server_database.sql' for sql file, 'shared/man_db_msg.proto' for proto file. 
If you don't want it:
    Usage: gen_proto sql_filename proto_filename'''
        
        dir = os.path.dirname(os.path.abspath(__file__)) + '/'
        sql_filename = dir + 'config/server_database.sql'
        proto_filename = 'shared/man_db_msg.proto'
    else:
        sql_filename = args[0]
        proto_filename = args[1]
    db = sql_sparser.get_db_struct(sql_filename)
    p = db2proto_gen.protogen()
    p.from_db(db, proto_filename)
    print '-'*50 + '\n'


@task
@consume_args
def gen_orm(args):
    '''Generate orm from sql file'''
    print '-'*50
    print 'Generating orm\n'
    
    dir = os.path.dirname(os.path.abspath(__file__)) + '/'
    if len(args) < 2:
        print '''Using 'config/server_database.sql' for sql file, 'shared/orm.py' for orm file. 
If you don't want it:
    Usage: gen_orm sql_filename orm_filename'''
        orm_filename = dir + 'shared/orm.py'
        sql_filename = dir + 'config/server_database.sql'
    else: 
        sql_filename = args[0]
        orm_filename = args[1]
        
    orm_file = open(orm_filename, 'w')
    db = sql_sparser.get_db_struct(sql_filename)

    res_str = db2orm_gen.generate_classes(db)
    orm_file.write(res_str)
    orm_file.close()
    print '-'*50 + '\n'

   
@task
@consume_args
def gen_proto_python(args):
    '''Generate python files for protobuf'''
    print '-'*50
    print 'Generating python files from .proto\n'
    
    dir = os.path.dirname(os.path.abspath(__file__)) + '/'
    if len(args) < 2: 
        print '''Using 'shared' for proto_path and 'man_db_msg.proto' for proto_filename. 
If you don't want it:
    Usage: gen_proto_python proto_path proto_filename'''
        
        proto_path = dir + 'shared/'
        proto_filename = proto_path + 'man_db_msg.proto'
    else:
        proto_path = args[0]
        proto_filename = proto_path + args[1]
        
    os.system('protoc --proto_path=%s --python_out=%s %s' % (proto_path, proto_path, proto_filename))
    print '-'*50 + '\n'

@task
@needs('gen_proto', 'gen_orm', 'gen_proto_python')
@consume_args
def generate_all():
    '''Generate proto, orm and python protobuf with default settings'''
    pass

def recreate_db(args):
    '''Recreate database indicated in args (server or node)'''
    print '-'*50
    print 'Recreating database for %s\n' % args[0]

    if not args: print 'Usage: recreate_db server|(node <db>)'
    else:
        if args[0] == 'server':
            conf = shared.config.Config(os.path.dirname(os.path.abspath(__file__)) + '/config/server_config.yml')
            
            sql_filename = os.path.dirname(os.path.abspath(__file__)) + '/config/server_database.sql'
            user = conf['database']['user']
            password = conf['database']['password']

            os.system('mysql --user=%s --password=%s < %s' % (user, password, sql_filename))
        elif args[0] == 'node':
            if len(args) < 2: print 'Usage: recreate_db node <db>'
            else:
                sql_filename = os.path.dirname(os.path.abspath(__file__)) + '/config/node_database.sql'
                db_name = args[1]

                if os.path.isfile(db_name):
                    if len(args) > 2 and (args[2] == '--force' or args[2] == '-f'):
                        os.remove(db_name)
                        print 'Old database deleted'
                    else: 
                        print 'Rewrite old "%s" file? (Y/n)' % db_name,
                        ans = raw_input('-> ')

                        if ans == 'Y' or ans == 'y' or ans == '':
                            os.remove(db_name)
                            print 'Old database deleted'
                        else:
                            print 'Recreating canceled'
                            print '-'*50 + '\n'
                            return

                os.system('sqlite3 %s < %s' % (db_name, sql_filename))
    
    print '-'*50 + '\n'

_recreate_db = task(consume_args(recreate_db))
recreate_db, _recreate_db = _recreate_db, recreate_db

@task
def clean_shared():
    '''Clean all shared files'''
    
    os.system('rm shared/orm.py shared/*.proto shared/*pb2.py')

