#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'Usage: reacreate_node_database.py <database name>'
        sys.exit(13)

    sql_filename = os.path.dirname(os.path.abspath(__file__)) + '/../config/node_database.sql'
    db_name = sys.argv[1]

    os.system('sqlite3 %s < %s' % (db_name, sql_filename))

