import shared.database as db
import datetime

with db.manager().transaction():
    users = []
            
    for i in range(0, 3):
        users.append(db.User())

    special_user = db.User()

    db.EmMarineCard(code = 100500, user = special_user.id)
    db.EmMarineCard(code = 123, user = users[1].id)
    db.EmMarineCard(code = 321, user = users[2].id)

    n = db.Node()
    n1 = db.Node()
    c1n1 = db.Controller(address = 123, node = n.id)
    c2n1 = db.Controller(address = 223, node = n.id)
    c3n1 = db.Controller(address = 323, node = n.id)
    c4 = db.Controller(address = 1232, node = n1.id)
    db.Controller(address = 423, node = n.id)
    db.Controller(address = 123, node = n1.id)
    db.Controller(address = 223, node = n1.id)
    db.Controller(address = 323, node = n1.id)
    db.Controller(address = 423, node = n1.id)
    p1 = db.Point(type = 'door', reader = 'em_marine_card', controller = c1n1)
    p2 = db.Point(type = 'door', reader = 'em_marine_card', controller = c1n1)
    p3 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = c2n1)
    p4 = db.Point(type = 'turnstile', reader = 'em_marine_card', controller = c3n1)
    p5 = db.Point(type = 'elevator', reader = 'em_marine_card', controller = c4)

    pg1 = db.PointGroup(name = 'entrance')
    pg2 = db.PointGroup(name = 'elevator')

    db.PointGroupLink(point = p1.id, point_group = pg1.id)
    db.PointGroupLink(point = p2.id, point_group = pg1.id)
    db.PointGroupLink(point = p3.id, point_group = pg1.id)
    db.PointGroupLink(point = p4.id, point_group = pg1.id)
    db.PointGroupLink(point = p5.id, point_group = pg2.id)

    user_groups = []
    for i in range(1, 100):
    	user_groups.append(db.UserGroup(name = 'test' + str(i)))

    for user in users[1:]:
    	for ug in user_groups:
    		db.UserGroupLink(user = user.id, user_group = ug.id)

    special_group = db.UserGroup(name = 'qwer')
    db.UserGroupLink(user = users[1].id, user_group = special_group.id)

    for i in range(1, 300):
        db.Log(time = datetime.datetime.now(), decision = 'allow_lim_time', point = p4.id, code = 100500, code_type = 'em_marine_card')

    true_script = db.Script(condition_code = "def check():\n\treturn True")
    random_50_50 = db.Script(condition_code = "import random\nrandom.seed(None)\ndef check():\n\tchoices = [True, False]\n\treturn random.choice(choices)")
    random_20_80 = db.Script(condition_code = "import random\nrandom.seed(None)\ndef check():\n\tchoices = [True, False, False, False, False]\n\treturn random.choice(choices)")
    wrong = db.Script(condition_code = "qwerty")

    true_group = user_groups[0]
    random_50_50_group = user_groups[1]
    random_20_80_group = user_groups[2]
    wrong_group = user_groups[3]

    #db.UserGroupLink(user = special_user.id, user_group = true_group)
    #db.UserGroupLink(user = special_user.id, user_group = random_50_50_group)
    #db.UserGroupLink(user = special_user.id, user_group = random_20_80_group)
    db.UserGroupLink(user = special_user.id, user_group = wrong_group)

    db.Access(point_group = pg1.id, user_group = true_group.id, script = true_script.id)
    db.Access(point_group = pg1.id, user_group = random_50_50_group.id, script = random_50_50.id)
    db.Access(point_group = pg1.id, user_group = random_20_80_group.id, script = random_20_80.id)
    db.Access(point_group = pg1.id, user_group = wrong_group.id, script = wrong.id)
    db.Access(point_group = pg2.id, user_group = user_groups[0].id, script = true_script.id)
    db.Access(point_group = pg2.id, user_group = special_group.id, script = true_script.id)

