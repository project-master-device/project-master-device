import re

# 22.10.2010
# 23.10.2010
# supposed to work only with create files from mysql_WB
# by default: PRIMARY KEY (`id`)

#Licence: BSD/LGPL on generators&parsers?

"""v0.2
result struct:
[{'table': 'test', 
  'f_keys': [{'table': 'ftable', 'db': 'MAN', 'key': 'xyz_id', 'f_key': 'id'}], 
  'db': 'MAN', 
  'columns': [
        ('abd', 'INT', {'default': None, 'unsigned': True, 'value': None}), 
        ('efg', 'VARCHAR', {'default': 'NULL', 'unsigned': None, 'value': '100'}), 
        ('qwe', 'ENUM', {'default': 'NULL', 'unsigned': None, 'value': ['male', 'female']}
        ('xyz_id', 'INT', {'default': None, 'unsigned': True, 'value': None})
        ]
}, ]
supports UNSIGNED, Data Type with value, DEFAULT, FOREIGN KEY
"""
def get_db_struct(sql_create_file_path):
    sql_source = open(sql_create_file_path, "r" )
    create_r = re.compile("^ *CREATE +TABLE +.*`(?P<db>\w+)`\.`(?P<table>\w+)` *\($")
    column_r = re.compile("^ *`(?P<name>\w+)` +(?P<data_type>[\w]+)(\((?P<data_type_val>[\w ,']*)\))?\
 *( ((?P<unsigned>UNSIGNED)|(DEFAULT (?P<default>[\w.]+))|(\w+)))* *,$")
    foreign_key_r = (re.compile("^ *FOREIGN KEY \(`(?P<key>\w+)` *\)$"),
                    re.compile("^ *REFERENCES `(?P<db>\w+)`.`(?P<table>\w+)` *\(`(?P<f_key>\w+)` *\)$"))
    result = []
    current_table = {}
    f_key_name = ''
    for line in sql_source:
        create_res = create_r.match(line)
        column_res = column_r.match(line)
        foreign_key_res = (foreign_key_r[0].match(line), foreign_key_r[1].match(line))
        if (create_res):
            current_table = {'db': create_res.group('db'), \
                            'table': create_res.group('table'), \
                            'columns': [], 'f_keys': []}
            result.append(current_table)
        elif (column_res):
            params = {}
            if (column_res.group('unsigned')):
                params['unsigned'] = True
            else:
                params['unsigned'] = None
            params['default'] = column_res.group('default')
            data_type = column_res.group('data_type')
            data_type_val = column_res.group('data_type_val')
            if (data_type == 'ENUM'):
                #cut '
                values = []
                for val in re.split(',', data_type_val):
                    values.append(val[1:-1])
                params['value'] = values
            else:
                params['value'] = data_type_val
            new_column = (column_res.group('name'), data_type, params)
            current_table['columns'].append(new_column)
        elif (foreign_key_res[0]):
            f_key_name = foreign_key_res[0].group('key')
        elif (foreign_key_res[1]):
            f_key_def = {'key': f_key_name, \
                        'db': foreign_key_res[1].group('db'), \
                        'table': foreign_key_res[1].group('table'), \
                        'f_key': foreign_key_res[1].group('f_key')}
            current_table['f_keys'].append(f_key_def)
            f_key_name = ''
    sql_source.close()
    return result