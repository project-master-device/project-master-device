# 23.10.2010
# 31.10.2010

#Licence: BSD/LGPL on generators&parsers?

import string

"""v.0.2 db struct
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
v.0.2 protogen
"""
#class to generate .proto file from based on db struct
class protogen:
    def __init__(self):
        self.lvl = 0
        self.counter = [0] * 100
        self.enum_type = 'EnumType_'
        self.table_type = 'TableType_'
        self.db_msg_name = 'FullMANDB'
        self.types_dictionary = {
            'INT_U': 'uint32', 'BIGINT_U': 'uint64', 'TINYINT_U': 'uint32', 
            'SMALLINT_U': 'uint32', 'MEDIUMINT_U': 'uint32', 
            'INT': 'int32', 'BIGINT': 'int64', 'TINYINT': 'int32', 
            'SMALLINT': 'int32', 'MEDIUMINT': 'int32', 
            'DECIMAL': 'double', 'FLOAT': 'float', 'DOUBLE': 'double', 
            'ENUM': 'string', 'VARCHAR': 'string', 'TEXT': 'string', 'DATETIME': 'string'}
    def tab(self):
        return ' ' * 2 * self.lvl
    def w(self, s = ''):
        self.proto_file.write(self.tab() + s + '\n')
    def pos_w(self, s):
        self.counter[self.lvl] += 1
        self.w(s + " = " + str(self.counter[self.lvl]) + ";")
    def lvl_open(self, s):
        self.w(s + ' {')
        self.lvl += 1
    def lvl_close(self):
        self.counter[self.lvl] = 0
        self.lvl -= 1
        self.w("}")
    def from_db(self, db_struct, proto_file_path):
        self.proto_file = open(proto_file_path, "w+" )
        self.w()
        self.lvl_open('message man_message')
        self.pos_w('required string message_type')
        self.w()
        #const DB name:
        self.lvl_open('message ' + self.db_msg_name) #TODO:change to FullDB
        for table in db_struct:
            self.lvl_open('message ' + self.table_type + table['table'])
            for column in table['columns']:
                data_type = column[1]
                dict_type = self.types_dictionary[data_type]
                if (data_type.find('INT') != -1 and column[2]['unsigned']):
                    data_type = column[1] + '_U'
                if (dict_type != 'enum'):
                    self.pos_w('optional ' + dict_type + ' '+ column[0])
                else:
                    self.lvl_open(dict_type + ' ' + self.enum_type + column[0])
                    for enum_val in column[2]['value']:
                       self.pos_w(enum_val.upper())
                    self.lvl_close()
                    self.pos_w('optional ' + self.enum_type + column[0] + ' ' + column[0])
            self.lvl_close()
            self.pos_w('repeated ' + self.table_type + table['table'] + ' '+ table['table'])
            self.w()
        self.lvl_close() #self.db_msg_name
        self.pos_w('optional ' + self.db_msg_name + ' db')
        self.lvl_close() #man_message
        self.proto_file.close()
