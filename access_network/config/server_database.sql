SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `MAN` ;
CREATE SCHEMA IF NOT EXISTS `MAN` DEFAULT CHARACTER SET utf8 ;
USE `MAN` ;

-- -----------------------------------------------------
-- Table `MAN`.`node`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`node` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`node` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `ip` VARCHAR(15) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`controller`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`controller` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`controller` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `address` INT UNSIGNED NOT NULL ,
  `node_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `controller_node_fk` (`node_id` ASC) ,
  CONSTRAINT `controller_node_fk`
    FOREIGN KEY (`node_id` )
    REFERENCES `MAN`.`node` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`point`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`point` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`point` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `type` ENUM('door','elevator','turnstile') NOT NULL ,
  `controller_id` INT UNSIGNED NOT NULL ,
  `reader` ENUM('em_marine_card') NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `point_controller_fk` (`controller_id` ASC) ,
  CONSTRAINT `point_controller_fk`
    FOREIGN KEY (`controller_id` )
    REFERENCES `MAN`.`controller` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`user_group`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`user_group` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`user_group` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(100) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`user` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`user` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(100) NULL DEFAULT NULL ,
  `surname` VARCHAR(100) NULL DEFAULT NULL ,
  `patronymic` VARCHAR(100) NULL DEFAULT NULL ,
  `gender` ENUM('male','female') NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`em_marine_card`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`em_marine_card` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`em_marine_card` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `code` BIGINT UNSIGNED NOT NULL ,
  `user_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `em_marine_card_user_fk` (`user_id` ASC) ,
  CONSTRAINT `em_marine_card_user_fk`
    FOREIGN KEY (`user_id` )
    REFERENCES `MAN`.`user` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`point_group`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`point_group` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`point_group` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `name` VARCHAR(100) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`script`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`script` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`script` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `condition_code` TEXT NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `MAN`.`access`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`access` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`access` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `script_id` INT UNSIGNED NOT NULL ,
  `point_group_id` INT UNSIGNED NOT NULL ,
  `user_group_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `access_point_group_fk` (`point_group_id` ASC) ,
  INDEX `access_user_group_fk` (`user_group_id` ASC) ,
  INDEX `access_script_fk` (`script_id` ASC) ,
  CONSTRAINT `access_point_group_fk`
    FOREIGN KEY (`point_group_id` )
    REFERENCES `MAN`.`point_group` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `access_user_group_fk`
    FOREIGN KEY (`user_group_id` )
    REFERENCES `MAN`.`user_group` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `access_script_fk`
    FOREIGN KEY (`script_id` )
    REFERENCES `MAN`.`script` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`point_group_link`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`point_group_link` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`point_group_link` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `point_id` INT UNSIGNED NOT NULL ,
  `point_group_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `PRIMARY2` (`point_id` ASC, `point_group_id` ASC) ,
  INDEX `point_group_link_point_fk` (`point_id` ASC) ,
  INDEX `point_group_link_point_group_fk` (`point_group_id` ASC) ,
  CONSTRAINT `point_group_link_point_fk`
    FOREIGN KEY (`point_id` )
    REFERENCES `MAN`.`point` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `point_group_link_point_group_fk`
    FOREIGN KEY (`point_group_id` )
    REFERENCES `MAN`.`point_group` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`user_group_link`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`user_group_link` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`user_group_link` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `user_id` INT UNSIGNED NOT NULL ,
  `user_group_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `user_group_link_user_fk` (`user_id` ASC) ,
  INDEX `user_group_link_user_group_fk` (`user_group_id` ASC) ,
  CONSTRAINT `user_group_link_user_fk`
    FOREIGN KEY (`user_id` )
    REFERENCES `MAN`.`user` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `user_group_link_user_group_fk`
    FOREIGN KEY (`user_group_id` )
    REFERENCES `MAN`.`user_group` (`id` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`log` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`log` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `time` DATETIME NOT NULL ,
  `decision` ENUM('allow_lim_time','allow_unlim_time','close','deny') NOT NULL ,
  `point_id` INT UNSIGNED NOT NULL ,
  `code` BIGINT UNSIGNED NULL ,
  `code_type` ENUM('em_marine_card') NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `log_point_fk` (`point_id` ASC) ,
  CONSTRAINT `log_point_fk`
    FOREIGN KEY (`point_id` )
    REFERENCES `MAN`.`point` (`id` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `MAN`.`database_send_log`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `MAN`.`database_send_log` ;

CREATE  TABLE IF NOT EXISTS `MAN`.`database_send_log` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT ,
  `begin_time` DATETIME NOT NULL ,
  `end_time` DATETIME NULL DEFAULT NULL ,
  `node_id` INT UNSIGNED NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `database_send_log_node_fk` (`node_id` ASC) ,
  CONSTRAINT `database_send_log_node_fk`
    FOREIGN KEY (`node_id` )
    REFERENCES `MAN`.`node` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
