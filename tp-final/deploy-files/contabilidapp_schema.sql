-- MySQL dump 10.13  Distrib 5.7.22, for Linux (x86_64)
--
-- Host: localhost    Database: contabilidapp
-- ------------------------------------------------------
-- Server version	5.7.22-0ubuntu0.16.04.1

CREATE DATABASE contabilidapp;
USE contabilidapp;

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
-- Table structure for table `CLIENTS`
--

DROP TABLE IF EXISTS `CLIENTS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `CLIENTS` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `CLIENT_CODE` varchar(255) DEFAULT NULL,
  `MIDDLE_CODE` varchar(255) DEFAULT NULL,
  `PREFIX_CODE` varchar(255) DEFAULT NULL,
  `SUFFIX_CODE` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `INPUT_FILES`
--

DROP TABLE IF EXISTS `INPUT_FILES`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `INPUT_FILES` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `FILE_DATA` longblob,
  `UPLOAD_PERIOD_ID` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FKrupra2lnabgld02auaukeemit` (`UPLOAD_PERIOD_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `INPUT_FILES_TRANSACTIONS`
--

DROP TABLE IF EXISTS `INPUT_FILES_TRANSACTIONS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `INPUT_FILES_TRANSACTIONS` (
  `InputFile_ID` bigint(20) NOT NULL,
  `transactions_ID` bigint(20) NOT NULL,
  UNIQUE KEY `UK_411cv7et7yjb1dpnnu6auxsfa` (`transactions_ID`),
  KEY `FKf7e4b541nswggyw3phhufv7ck` (`InputFile_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TRANSACTIONS`
--

DROP TABLE IF EXISTS `TRANSACTIONS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TRANSACTIONS` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `AMOUNT` decimal(19,2) DEFAULT NULL,
  `CLIENT_ID` bigint(20) DEFAULT NULL,
  `INPUT_FILE_ID` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `FKm053y7486ga6akw986t7phlpe` (`CLIENT_ID`),
  KEY `FKqou4c12cdbj6ff3tto9611kag` (`INPUT_FILE_ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UPLOAD_PERIODS`
--

DROP TABLE IF EXISTS `UPLOAD_PERIODS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UPLOAD_PERIODS` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `OPEN` bit(1) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-07-24 19:23:10
