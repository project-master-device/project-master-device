# Copyright (c) 2010-2011 Timofey Kirillov
#
# This file is part of Project Master Device.
#
# Project Master Device is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Project Master Device is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Project Master Device.  If not, see <http://www.gnu.org/licenses/>.


import unittest
import pavement
import node.points_manager as npm
import shared.database as db

class TestPointsManager(unittest.TestCase):
    def setUp(self):
        print '\n== test started =='
        pavement._recreate_db(('node', 'test.db', '--force'))
        
        import node.main
        # Reconnect to the database if
        # setUp called twice or more times
        reload(node.main)

    def tearDown(self):
        print '\n== test finished =='

    def fill_db(self):
        with db.manager().transaction():
            n = db.Node()
            c1 = db.Controller(address = 1, node = n.id)
            c2 = db.Controller(address = 2, node = n.id)
            c3 = db.Controller(address = 3, node = n.id)
            p1 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = c1.id)
            p2 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = c2.id)
            p3 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = c3.id)

    def test_initialize(self):
        try:
            self.fill_db()
            npm.PointsManager()

        except Exception, exc:
            self.fail(exc)


class TestAccessPoint(unittest.TestCase):
    def setUp(self):
        print '\n== test started =='
        pavement._recreate_db(('node', 'test.db', '--force'))

        import node.main
        # Reconnect to the database if
        # setUp called twice or more times
        reload(node.main)

    def tearDown(self):
        print '== test finished =='

    def fill_db(self):
        with db.manager().transaction():
            self.n = db.Node()
            self.c = db.Controller(address = 1, node = self.n.id)
            self.p1 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = self.c.id)
            self.p2 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = self.c.id)
            self.pg = db.PointGroup(name = 'entrance')
            db.PointGroupLink(point = self.p1.id, point_group = self.pg.id)
            db.PointGroupLink(point = self.p2.id, point_group = self.pg.id)

            self.u = db.User()
            self.emc = db.EmMarineCard(code = 100500, user = self.u.id)
            self.ug = db.UserGroup(name = 'admin')
            db.UserGroupLink(user = self.u.id, user_group = self.ug.id)

            self.true_script = db.Script(condition_code = "def check():\n\treturn True")
            self.false_script = db.Script(condition_code = "def check():\n\treturn False")
            self.wrong_script = db.Script(condition_code = "qwerty")
    
    def test_check_access_empty_base(self):
        try:
            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(100500, 1))

        except Exception, exc:
            self.fail(exc)

    def test_check_access_not_allowed_code1(self):
        try:
            self.fill_db()

            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p1.id))
            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p2.id))

        except Exception, exc:
            self.fail(exc)

    def test_check_access_not_allowed_code2(self):
        try:
            self.fill_db()
           
            with db.manager().transaction():
                db.Access(script = self.false_script.id, user_group = self.ug.id, point_group = self.pg.id)

            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p1.id))
            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p2.id))

        except Exception, exc:
            self.fail(exc)

    def test_check_access_wrong_code(self):
        try:
            self.fill_db()

            with db.manager().transaction():
                db.Access(script = self.wrong_script.id, user_group = self.ug.id, point_group = self.pg.id)

            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p1.id))
            self.assertFalse(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p2.id))

        except Exception, exc:
            self.fail(exc)

    def test_check_access_allowed_code(self):
        try:
            self.fill_db()

            with db.manager().transaction():
                db.Access(script = self.true_script.id, user_group = self.ug.id, point_group = self.pg.id)
                db.Access(script = self.false_script.id, user_group = self.ug.id, point_group = self.pg.id)
                db.Access(script = self.wrong_script.id, user_group = self.ug.id, point_group = self.pg.id)

            self.assertTrue(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p1.id))
            self.assertTrue(npm.AccessPoint._AccessPoint__check_access(self.emc.code, self.p2.id))

        except Exception, exc:
            self.fail(exc)


if __name__ == '__main__':
    unittest.main()

