#!/usr/bin/env python

import os

import sql_sparser
import db2proto_gen
import db2orm_gen

def gen_proto(sql_filename, proto_filename, db):
    p = db2proto_gen.protogen()
    p.from_db(db, proto_filename)

def gen_orm(orm_filename, db):
    orm_file = open(orm_filename, 'w')
    
    res_str = db2orm_gen.generate_classes(db)
    
    orm_file.write(res_str)
    orm_file.close()

def gen_proto_python(proto_path, proto_filename):
    os.system('protoc --proto_path=%s --python_out=%s %s' % (proto_path, proto_path, proto_filename))

if __name__ == '__main__':
    tool_dir = os.path.dirname(os.path.abspath(__file__)) + '/'
    sql_filename = tool_dir + '../config/server_database.sql'
    orm_filename = tool_dir + '../shared/orm.py'
    proto_path = tool_dir + '../shared/'
    proto_filename = proto_path + 'man_db_msg.proto'

    db = sql_sparser.get_db_struct(sql_filename)
    
    gen_proto(sql_filename, proto_filename, db)
    gen_orm(orm_filename, db)
    gen_proto_python(proto_path, proto_filename)

