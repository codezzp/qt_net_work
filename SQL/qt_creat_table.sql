CREATE DATABASE IF NOT EXISTS db_qt;
use db_qt;

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;


DROP TABLE IF EXISTS `qt_uart`;
CREATE TABLE `qt_uart`  (
  `q_id` int NOT NULL AUTO_INCREMENT COMMENT 'id',
  `date` datetime NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `config_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `file` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`q_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 39 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;


DROP TRIGGER IF EXISTS `before_insert`;
delimiter ;;
CREATE TRIGGER `before_insert` BEFORE INSERT ON `qt_uart` FOR EACH ROW set new.date = NOW()
;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;