import sqlobject

class Node(sqlobject.SQLObject):
	class sqlmeta:
		table = 'node'

	ip = sqlobject.StringCol(length = 15, default = None)
	controller_mk = sqlobject.MultipleJoin('Controller')
	database_send_log_mk = sqlobject.MultipleJoin('DatabaseSendLog')


class Controller(sqlobject.SQLObject):
	class sqlmeta:
		table = 'controller'

	address = sqlobject.IntCol()
	node = sqlobject.ForeignKey('Node')
	point_mk = sqlobject.MultipleJoin('Point')


class Point(sqlobject.SQLObject):
	class sqlmeta:
		table = 'point'

	type = sqlobject.EnumCol(enumValues = ['door', 'elevator', 'turnstile'])
	controller = sqlobject.ForeignKey('Controller')
	reader = sqlobject.EnumCol(enumValues = ['em_marine_card'])
	point_group_link_mk = sqlobject.MultipleJoin('PointGroupLink')
	log_mk = sqlobject.MultipleJoin('Log')


class UserGroup(sqlobject.SQLObject):
	class sqlmeta:
		table = 'user_group'

	name = sqlobject.StringCol(length = 100)
	access_mk = sqlobject.MultipleJoin('Access')
	user_group_link_mk = sqlobject.MultipleJoin('UserGroupLink')


class User(sqlobject.SQLObject):
	class sqlmeta:
		table = 'user'

	name = sqlobject.StringCol(length = 100, default = None)
	surname = sqlobject.StringCol(length = 100, default = None)
	patronymic = sqlobject.StringCol(length = 100, default = None)
	gender = sqlobject.EnumCol(enumValues = ['male', 'female'], default = None)
	em_marine_card_mk = sqlobject.MultipleJoin('EmMarineCard')
	user_group_link_mk = sqlobject.MultipleJoin('UserGroupLink')


class EmMarineCard(sqlobject.SQLObject):
	class sqlmeta:
		table = 'em_marine_card'

	code = sqlobject.BigIntCol()
	user = sqlobject.ForeignKey('User')


class PointGroup(sqlobject.SQLObject):
	class sqlmeta:
		table = 'point_group'

	name = sqlobject.StringCol(length = 100)
	access_mk = sqlobject.MultipleJoin('Access')
	point_group_link_mk = sqlobject.MultipleJoin('PointGroupLink')


class Script(sqlobject.SQLObject):
	class sqlmeta:
		table = 'script'

	condition_code = sqlobject.StringCol()
	access_mk = sqlobject.MultipleJoin('Access')


class Access(sqlobject.SQLObject):
	class sqlmeta:
		table = 'access'

	script = sqlobject.ForeignKey('Script')
	point_group = sqlobject.ForeignKey('PointGroup')
	user_group = sqlobject.ForeignKey('UserGroup')


class PointGroupLink(sqlobject.SQLObject):
	class sqlmeta:
		table = 'point_group_link'

	point = sqlobject.ForeignKey('Point')
	point_group = sqlobject.ForeignKey('PointGroup')


class UserGroupLink(sqlobject.SQLObject):
	class sqlmeta:
		table = 'user_group_link'

	user = sqlobject.ForeignKey('User')
	user_group = sqlobject.ForeignKey('UserGroup')


class Log(sqlobject.SQLObject):
	class sqlmeta:
		table = 'log'

	time = sqlobject.DateTimeCol()
	decision = sqlobject.EnumCol(enumValues = ['allow_lim_time', 'allow_unlim_time', 'close', 'deny'])
	point = sqlobject.ForeignKey('Point')
	code = sqlobject.BigIntCol()
	code_type = sqlobject.EnumCol(enumValues = ['em_marine_card'])


class DatabaseSendLog(sqlobject.SQLObject):
	class sqlmeta:
		table = 'database_send_log'

	begin_time = sqlobject.DateTimeCol()
	end_time = sqlobject.DateTimeCol(default = None)
	node = sqlobject.ForeignKey('Node')


