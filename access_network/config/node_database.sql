-- Authors: Sergey Gnuskov, Gennady Kalashnikov, Timofey Kirillov
-- Creator:       MySQL Workbench 5.2.28/ExportSQLite plugin 2009.12.02
PRAGMA foreign_keys = OFF;

-- Schema: MAN
BEGIN;
CREATE TABLE "node"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "ip" VARCHAR(15) DEFAULT NULL
);
CREATE TABLE "controller"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "address" INTEGER NOT NULL CHECK("address">=0),
  "node_id" INTEGER NOT NULL CHECK("node_id">=0),
  CONSTRAINT "controller_node_fk"
    FOREIGN KEY("node_id")
    REFERENCES "node"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "controller.controller_node_fk" ON "controller"("node_id");
CREATE TABLE "user_group"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "name" VARCHAR(100) NOT NULL
);
CREATE TABLE "user"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "name" VARCHAR(100) DEFAULT NULL,
  "surname" VARCHAR(100) DEFAULT NULL,
  "patronymic" VARCHAR(100) DEFAULT NULL,
  "gender" TEXT CHECK("gender" IN('male','female')) DEFAULT NULL
);
CREATE TABLE "em_marine_card"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "code" INTEGER NOT NULL CHECK("code">=0),
  "user_id" INTEGER NOT NULL CHECK("user_id">=0),
  CONSTRAINT "em_marine_card_user_fk"
    FOREIGN KEY("user_id")
    REFERENCES "user"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "em_marine_card.em_marine_card_user_fk" ON "em_marine_card"("user_id");
CREATE TABLE "point_group"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "name" VARCHAR(100) NOT NULL
);
CREATE TABLE "user_group_link"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "user_id" INTEGER NOT NULL CHECK("user_id">=0),
  "user_group_id" INTEGER NOT NULL CHECK("user_group_id">=0),
  CONSTRAINT "user_group_link_user_fk"
    FOREIGN KEY("user_id")
    REFERENCES "user"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "user_group_link_user_group_fk"
    FOREIGN KEY("user_group_id")
    REFERENCES "user_group"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "user_group_link.user_group_link_user_fk" ON "user_group_link"("user_id");
CREATE INDEX "user_group_link.user_group_link_user_group_fk" ON "user_group_link"("user_group_id");
CREATE TABLE "database_send_log"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "begin_time" DATETIME NOT NULL,
  "end_time" DATETIME DEFAULT NULL,
  "node_id" INTEGER NOT NULL CHECK("node_id">=0),
  CONSTRAINT "database_send_log_node_fk"
    FOREIGN KEY("node_id")
    REFERENCES "node"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "database_send_log.database_send_log_node_fk" ON "database_send_log"("node_id");
CREATE TABLE "script"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "condition_code" TEXT NOT NULL
);
CREATE TABLE "point"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "type" TEXT NOT NULL CHECK("type" IN('door','elevator','turnstile')),
  "controller_id" INTEGER NOT NULL CHECK("controller_id">=0),
  "reader" TEXT CHECK("reader" IN('em_marine_card')),
  CONSTRAINT "point_controller_fk"
    FOREIGN KEY("controller_id")
    REFERENCES "controller"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "point.point_controller_fk" ON "point"("controller_id");
CREATE TABLE "access"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "script_id" INTEGER NOT NULL CHECK("script_id">=0),
  "point_group_id" INTEGER NOT NULL CHECK("point_group_id">=0),
  "user_group_id" INTEGER NOT NULL CHECK("user_group_id">=0),
  CONSTRAINT "access_point_group_fk"
    FOREIGN KEY("point_group_id")
    REFERENCES "point_group"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "access_user_group_fk"
    FOREIGN KEY("user_group_id")
    REFERENCES "user_group"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "access_script_fk"
    FOREIGN KEY("script_id")
    REFERENCES "script"("id")
);
CREATE INDEX "access.access_point_group_fk" ON "access"("point_group_id");
CREATE INDEX "access.access_user_group_fk" ON "access"("user_group_id");
CREATE INDEX "access.access_script_fk" ON "access"("script_id");
CREATE TABLE "point_group_link"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "point_id" INTEGER NOT NULL CHECK("point_id">=0),
  "point_group_id" INTEGER NOT NULL CHECK("point_group_id">=0),
  CONSTRAINT "point_group_link_point_fk"
    FOREIGN KEY("point_id")
    REFERENCES "point"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT "point_group_link_point_group_fk"
    FOREIGN KEY("point_group_id")
    REFERENCES "point_group"("id")
    ON DELETE CASCADE
    ON UPDATE RESTRICT
);
CREATE INDEX "point_group_link.PRIMARY2" ON "point_group_link"("point_id","point_group_id");
CREATE INDEX "point_group_link.point_group_link_point_fk" ON "point_group_link"("point_id");
CREATE INDEX "point_group_link.point_group_link_point_group_fk" ON "point_group_link"("point_group_id");
CREATE TABLE "log"(
  "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL CHECK("id">=0),
  "time" DATETIME NOT NULL,
  "decision" TEXT NOT NULL CHECK("decision" IN('allow_lim_time','allow_unlim_time','close','deny')),
  "point_id" INTEGER NOT NULL CHECK("point_id">=0),
  "code" INTEGER CHECK("code">=0),
  "code_type" TEXT CHECK("code_type" IN('em_marine_card'))
);
CREATE INDEX "log.log_point_fk" ON "log"("point_id");
COMMIT;
