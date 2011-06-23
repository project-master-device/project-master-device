#!/usr/bin/env python
# -*- coding: utf-8 -*- 

import sys

import sql_sparser

sql_orm_types = {'INT': 'IntCol', 'BIGINT': 'BigIntCol', 'ENUM': 'EnumCol', 'VARCHAR': 'StringCol', 'TEXT': 'StringCol', 'DATETIME': 'DateTimeCol'}
sql_orm_types_args = {'VARCHAR': 'length', 'ENUM': 'enumValues'}
sql_orm_values = {'NULL': 'None'}

class ClassGenerator:
    def __init__(self, table_schema = {}, mk_tables = [], indent_level = 0, types_dict = sql_orm_types, types_args_dict = sql_orm_types_args, values_dict = sql_orm_values, indent_string = '\t'):
        self._table_schema = table_schema
        self._mk_tables = mk_tables
        self._types_dict = types_dict
        self._types_args_dict = types_args_dict
        self._values_dict = values_dict
        self._indent_level = indent_level
        self._indent_string = indent_string
        self.generate_fk_attrs_list()

    def generate_fk_attrs_list(self):
        if self._table_schema: self._fk_attrs_list = [fk_descriptor['key'] for fk_descriptor in self._table_schema['f_keys']]

    @staticmethod
    def import_generate():
        res = 'import sqlobject\n'
        
        return res

    def get_absolute_indent(self, level):
        res = self._indent_string * (self._indent_level + level)
        
        return res

    def is_fk(self, attr_name):
        if attr_name in self._fk_attrs_list: return True
        
        return False

    def get_fk_descriptor(self, attr_name):
        res = dict()
        for fk_descriptor in self._table_schema['f_keys']:
            if attr_name == fk_descriptor['key']:
                res = fk_descriptor

        return res

    # Generate class name based on table name.
    def class_name_generate(self, table_name):
        words = table_name.split('_')
        # Convert each word to the title form and concatenate words.
        res = reduce( (lambda sum, word: sum + word), [word.title() for word in words] )

        return res

    # Generate class header defenition.
    def class_header_generate(self, table_name):
        res = 'class ' + self.class_name_generate(table_name) + '(sqlobject.SQLObject):\n'

        return res
    
    # Generate attribute defenition.
    def attr_generate(self, column):
        res = str()

        if self.is_fk(column[0]):
            res += self.fk_name_generate(column[0]) + ' = sqlobject.'
            res += 'ForeignKey('
            fk_descriptor = self.get_fk_descriptor(column[0])
            res += '\'' + self.class_name_generate(fk_descriptor['table']) + '\''
            res += ')'
        else:
            res += column[0] + ' = sqlobject.'
            res += self._types_dict[column[1]] + '('

            type_arg = str()
            if column[2]['value']:
                type_arg += self._types_args_dict[column[1]] + ' = ' + str(column[2]['value'])

            default_arg = str()
            if column[2]['default']:
                default_arg += 'default = ' + self._values_dict[column[2]['default']]

            res += type_arg + (type_arg and default_arg and ', ') + default_arg
            res += ')'

        res += '\n'

        return res

    def fk_name_generate(self, column_name):
        res = column_name[:-3]
        
        return res

    def mk_name_generate(self, ref_table_name):
        res = ref_table_name + '_mk'
        
        return res

    def mk_generate(self, ref_table_name):
        res = self.mk_name_generate(ref_table_name) + ' = sqlobject.MultipleJoin(\''
        res += self.class_name_generate(ref_table_name) + '\')'
        res += '\n'

        return res

    def sqlmeta_class_header_generate(self, table_name):
        res = 'class sqlmeta:\n'
        
        return res
    
    def sqlmeta_attr_table_generate(self, table_name):
        res = 'table = ' + '\'' + table_name + '\'\n'

        return res

    # Return string with the full class defenition !@#!@#!@#!@ relative to the specified nesting level code.
    def generate(self):
        res = str()
        res += self.get_absolute_indent(0) + self.class_header_generate(self._table_schema['table'])
        res += self.get_absolute_indent(1) + self.sqlmeta_class_header_generate(self._table_schema['table'])
        res += self.get_absolute_indent(2) + self.sqlmeta_attr_table_generate(self._table_schema['table'])
        res += '\n'

        for column in self._table_schema['columns']:
            if column[0] != 'id':
                res += self.get_absolute_indent(1) + self.attr_generate(column)

        for ref_table_name in self._mk_tables:
            res += self.get_absolute_indent(1) + self.mk_generate(ref_table_name) 

        return res

    def set_table_schema(self, table_schema):   
        self._table_schema = table_schema
        self.generate_fk_attrs_list()

    def set_mk_tables(self, mk_tables):
        self._mk_tables = mk_tables


def get_mk_tables(table_schema, db_schema):
    res = []

    for ts in db_schema:
        for fk_descriptor in ts['f_keys']:
            if fk_descriptor['table'] == table_schema['table']:
                res.append(ts['table'])

    return res

def generate_classes(db_schema):
    res = ClassGenerator.import_generate() + '\n'

    cg = ClassGenerator()
    for table_schema in db_schema:
        cg.set_table_schema(table_schema)
        cg.set_mk_tables(get_mk_tables(table_schema, db_schema))
        res += cg.generate()
        res += '\n\n'

    return res

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print "Usage: python orm_generator.py <sql dump file> <destination file>"
        sys.exit(13)

    sql_file_name = sys.argv[1]
    dest_file_name = sys.argv[2]
    
    dest_file = open(dest_file_name, 'w')

    db_schema = sql_sparser.get_db_struct(sql_file_name)
    res_str = generate_classes(db_schema)
    
    dest_file.write(res_str)
    dest_file.close()
