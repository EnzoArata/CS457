--CS457 PA1

--Database metadata (20 points)
CREATE DATABASE db_1;
CREATE DATABASE db_1;
CREATE DATABASE db_2;
DROP DATABASE db_2;
DROP DATABASE db_2;
CREATE DATABASE db_2;

--Table metadata (50 points)
USE db_1;
CREATE TABLE tbl_1 (a1 int, a2 varchar(20));
CREATE TABLE tbl_1 (a3 float, a4 char(20));
DROP TABLE tbl_1;
DROP TABLE tbl_1;
CREATE TABLE tbl_1 (a1 int, a2 varchar(20));
SELECT * FROM tbl_1;
ALTER TABLE tbl_1 ADD a3 float;
SELECT * FROM tbl_1;
CREATE TABLE tbl_2 (a3 float, a4 char(20));
SELECT * FROM tbl_2;
USE db_2;
SELECT * FROM tbl_1;
CREATE TABLE tbl_1 (a3 float, a4 char(20));
SELECT * FROM tbl_1;


