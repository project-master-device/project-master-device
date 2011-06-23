#!/usr/bin/env python

import os

if __name__ == '__main__':
    sql_filename = os.path.dirname(os.path.abspath(__file__)) + '/../config/server_database.sql'

    os.system('mysql -u root < %s' % sql_filename)

