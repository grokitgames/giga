-- MySQL dump 10.13  Distrib 5.6.19, for osx10.9 (x86_64)
--
-- Host: localhost    Database: mmorpg
-- ------------------------------------------------------
-- Server version	5.6.19

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `asset`
--

DROP TABLE IF EXISTS `asset`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `asset` (
  `asset_id` int(11) NOT NULL AUTO_INCREMENT,
  `filename` text NOT NULL,
  `type` int(10) unsigned NOT NULL,
  `aabb_min` varchar(100) NOT NULL,
  `aabb_max` varchar(100) NOT NULL,
  PRIMARY KEY (`asset_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `asset`
--

LOCK TABLES `asset` WRITE;
/*!40000 ALTER TABLE `asset` DISABLE KEYS */;
INSERT INTO `asset` VALUES (1,'room.g3d',1,'(-5.0, -5.0, -5.0)','(5.0, 5.0, 5.0)'),(2,'crate.g3d',1,'(-0.5, -0.5, -0.5)','(0.5, 0.5, 0.5)'),(3,'person.g3d',1,'(-0.5, -0.5, -1.0)','(0.5, 0.5, 1.0)');
/*!40000 ALTER TABLE `asset` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `camera`
--

DROP TABLE IF EXISTS `camera`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `camera` (
  `camera_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) DEFAULT NULL,
  `object_id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `position` varchar(100) NOT NULL,
  `look` varchar(100) NOT NULL,
  `fnear` double DEFAULT NULL,
  `ffar` double DEFAULT NULL,
  PRIMARY KEY (`camera_id`),
  KEY `ix_camera_scene_id` (`scene_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `camera`
--

LOCK TABLES `camera` WRITE;
/*!40000 ALTER TABLE `camera` DISABLE KEYS */;
INSERT INTO `camera` VALUES (1,1,4,'Camera','(3.634860, 1.000000, 2.102240)','(-0.794397, 0.000000, -0.607398)',0.1,100);
/*!40000 ALTER TABLE `camera` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `entity`
--

DROP TABLE IF EXISTS `entity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `entity` (
  `object_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) DEFAULT NULL,
  `name` text,
  PRIMARY KEY (`object_id`),
  KEY `ix_object_scene_id` (`scene_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `entity`
--

LOCK TABLES `entity` WRITE;
/*!40000 ALTER TABLE `entity` DISABLE KEYS */;
INSERT INTO `entity` VALUES (1,1,'Room'),(2,1,'Crate'),(3,1,'Light'),(4,1,'Camera'),(5,1,'Player');
/*!40000 ALTER TABLE `entity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `light`
--

DROP TABLE IF EXISTS `light`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `light` (
  `light_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) NOT NULL,
  `object_id` int(11) DEFAULT NULL,
  `type` int(11) NOT NULL,
  `color` varchar(255) NOT NULL,
  `position` varchar(100) NOT NULL,
  `attenuation` varchar(100) NOT NULL,
  PRIMARY KEY (`light_id`),
  KEY `ix_light_object_id` (`object_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `light`
--

LOCK TABLES `light` WRITE;
/*!40000 ALTER TABLE `light` DISABLE KEYS */;
INSERT INTO `light` VALUES (1,1,3,2,'(1.000000, 1.000000, 1.000000)','(0.850000, 1.500000, 1.000000)','(1.000000, 0.250000, 0.100000)');
/*!40000 ALTER TABLE `light` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player`
--

DROP TABLE IF EXISTS `player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player` (
  `player_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `scene_id` int(11) NOT NULL,
  `entity_id` int(11) NOT NULL,
  `position` varchar(255) NOT NULL,
  `look` varchar(255) NOT NULL,
  UNIQUE KEY `player_id` (`player_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player`
--

LOCK TABLES `player` WRITE;
/*!40000 ALTER TABLE `player` DISABLE KEYS */;
INSERT INTO `player` VALUES (1,2,1,5,'(3.749788, 1.000000, 2.834073)','(-0.760088, 0.000000, -0.649820)');
/*!40000 ALTER TABLE `player` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `renderable`
--

DROP TABLE IF EXISTS `renderable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `renderable` (
  `renderable_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) NOT NULL,
  `object_id` int(11) DEFAULT NULL,
  `asset_id` int(11) NOT NULL,
  `filename` varchar(100) NOT NULL,
  `position` varchar(100) NOT NULL,
  `rotation` varchar(100) NOT NULL,
  `shader` text,
  PRIMARY KEY (`renderable_id`),
  KEY `ix_renderable_object_id` (`object_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `renderable`
--

LOCK TABLES `renderable` WRITE;
/*!40000 ALTER TABLE `renderable` DISABLE KEYS */;
INSERT INTO `renderable` VALUES (1,1,1,1,'room.g3d','(0.000000, 0.000000, 0.000000)','(0.000000, 0.000000, 0.000000, 1.000000)','default'),(2,1,2,2,'crate.g3d','(-1.000000, 0.000000, -2.000000)','(0.000000, 0.000000, 0.000000, 1.000000)','default'),(3,1,5,3,'person.g3d','(3.000000, 0.000000, 3.000000)','(0.000000, 0.000000, 0.000000, 1.000000)','default');
/*!40000 ALTER TABLE `renderable` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rigidbody`
--

DROP TABLE IF EXISTS `rigidbody`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rigidbody` (
  `rigidbody_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) NOT NULL,
  `object_id` int(11) DEFAULT NULL,
  `mass` double DEFAULT NULL,
  `shape` text,
  `params` text,
  PRIMARY KEY (`rigidbody_id`),
  KEY `ix_rigidbody_object_id` (`object_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rigidbody`
--

LOCK TABLES `rigidbody` WRITE;
/*!40000 ALTER TABLE `rigidbody` DISABLE KEYS */;
INSERT INTO `rigidbody` VALUES (2,1,1,0,'static',''),(4,1,2,0,'static',''),(5,1,5,10,'character','(1.65, 0.5, 0.35)');
/*!40000 ALTER TABLE `rigidbody` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scene`
--

DROP TABLE IF EXISTS `scene`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scene` (
  `scene_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `ambient` varchar(100) NOT NULL,
  PRIMARY KEY (`scene_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scene`
--

LOCK TABLES `scene` WRITE;
/*!40000 ALTER TABLE `scene` DISABLE KEYS */;
INSERT INTO `scene` VALUES (1,'Test Scene','(0.05,0.05,0.05)');
/*!40000 ALTER TABLE `scene` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `script`
--

DROP TABLE IF EXISTS `script`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `script` (
  `script_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) NOT NULL,
  `object_id` int(11) DEFAULT NULL,
  `filename` text,
  PRIMARY KEY (`script_id`),
  KEY `ix_script_object_id` (`object_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `script`
--

LOCK TABLES `script` WRITE;
/*!40000 ALTER TABLE `script` DISABLE KEYS */;
INSERT INTO `script` VALUES (1,1,5,'player.js');
/*!40000 ALTER TABLE `script` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `session`
--

DROP TABLE IF EXISTS `session`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `session` (
  `session_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) unsigned NOT NULL,
  `secret_key` varchar(100) NOT NULL,
  `iv` varchar(100) NOT NULL,
  UNIQUE KEY `session_id` (`session_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `session`
--

LOCK TABLES `session` WRITE;
/*!40000 ALTER TABLE `session` DISABLE KEYS */;
INSERT INTO `session` VALUES (1,1,'D<#FCGHiWixIgGSEISax5lwunLFzhOkK','L<^#Cw\\>ErGLY<%e'),(2,2,'j%RzDbq$eo$!2zyR(MxGwjK)Z*9bhSCq','G(Q9)6W>|J7W7em0');
/*!40000 ALTER TABLE `session` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `user_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `email` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `entity_id` int(11) DEFAULT NULL,
  `last_login` bigint(20) NOT NULL,
  `active` tinyint(4) NOT NULL,
  UNIQUE KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (1,'terry@icedteapowered.com','$2y$10$gJB.cU8SHYFmQ28TXgi96OCg/P1LeRfu1eWd0elJKkEv5jczghDky',NULL,0,1),(2,'tj.hackin.smith@gmail.com','$2y$10$U396zv4xaHi/6pnrGvf2z.XaBOfO7ZeSinDFe.s9W72cCr038Jr2y',5,0,1);
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-11-04 14:56:34
