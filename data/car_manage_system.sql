-- --------------------------------------------------------
-- 主机:                           192.168.246.151
-- 服务器版本:                        8.0.31 - MySQL Community Server - GPL
-- 服务器操作系统:                      Linux
-- HeidiSQL 版本:                  12.7.0.6850
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- 导出 car_manage_system 的数据库结构
DROP DATABASE IF EXISTS `car_manage_system`;
CREATE DATABASE IF NOT EXISTS `car_manage_system` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `car_manage_system`;

-- 导出  表 car_manage_system.t_opt_log 结构
DROP TABLE IF EXISTS `t_opt_log`;
CREATE TABLE IF NOT EXISTS `t_opt_log` (
  `id` int NOT NULL AUTO_INCREMENT,
  `type` varchar(50) DEFAULT NULL,
  `action` varchar(50) DEFAULT NULL,
  `username` varchar(50) DEFAULT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `result` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- 正在导出表  car_manage_system.t_opt_log 的数据：~2 rows (大约)
DELETE FROM `t_opt_log`;
INSERT INTO `t_opt_log` (`id`, `type`, `action`, `username`, `start_time`, `end_time`, `result`) VALUES
	(1, '登录', '登录', 'admin', '2024-07-10 11:55:46', '2024-07-10 11:55:47', '登录成功'),
	(2, '登录', '登录', 'admin', '2024-07-10 11:55:46', '2024-07-10 11:55:47', '登录失败');

-- 导出  表 car_manage_system.t_user 结构
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE IF NOT EXISTS `t_user` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(512) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- 正在导出表  car_manage_system.t_user 的数据：~2 rows (大约)
DELETE FROM `t_user`;
INSERT INTO `t_user` (`id`, `username`, `password`) VALUES
	(1, 'root', '7014ea6ab6c645f45e7485ad63e27cffeed82697faecc1ff6805c86bf2f714ad'),
	(2, 'admin', '4a88af90ba5d7f46ff320c723d5281e1926b2a4fecccad65b0e494aac2ddf2ba');

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
