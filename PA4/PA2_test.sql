--CS457 PA2

--Construct the database and table (0 points; expected to work from PA1)
CREATE DATABASE CS457_PA2;
USE CS457_PA2;
CREATE TABLE Product (pid int, name varchar(20), price float);
CREATE TABLE bepis (pid int, weight int, height int, price float);


--Insert new data (20 points)
insert into Product values(1,	'Gizmo',      	19.99);
insert into Product values(2,	'PowerGizmo', 	29.99);
insert into Product values(3,	'SingleTouch',  149.99);
insert into Product values(4,	'MultiTouch', 	199.99);
insert into Product values(5,	'SuperGizmo', 	49.99);
insert into bepis values(1,	2, 3, 	49.99);
insert into bepis values(2,	12, 9,       20);
insert into bepis values(3,	20, 11,       24);

select * from Product;

select * from bepis;

--Modify data (20 points)
update Product  
set name = 'Gizmo' 
where name = 'SuperGizmo';

update Product  
set price = 14.99 
where name = 'Gizmo';

select * from product;

update bepis 
set weight = 50 
where height = 9;

update bepis 
set height = 3 
where price > 20;

select * from bepis;


--Delete data (20 points)
delete from product 
where name = 'PowerGizmo';

--delete from product 
--where name = 'Gizmo';

--delete from product 
--where price > 175.99;




select * from product;

select * from bepis;

--Query subsets (10 points)
select pid, name 
from product 
where pid != 3;


select weight, height 
from bepis
where weight != 3;
-- Expected output
--
-- Database CS457_PA2 created.
-- Using database CS457_PA2.
-- Table Product created.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- pid int|name varchar(20)|price float
-- 1|Gizmo|19.99
-- 2|PowerGizmo|29.99
-- 3|SingleTouch|149.99
-- 4|MultiTouch|199.99
-- 5|SuperGizmo|49.99
-- 1 record modified.
-- 2 records modified.
-- pid int|name varchar(20)|price float
-- 1|Gizmo|14.99
-- 2|PowerGizmo|29.99
-- 3|SingleTouch|149.99
-- 4|MultiTouch|199.99
-- 5|Gizmo|14.99
-- 2 records deleted.
-- 1 record deleted.
-- pid int|name varchar(20)|price float
-- 2|PowerGizmo|29.99
-- 3|SingleTouch|149.99
-- name varchar(20)|price float
-- SingleTouch|149.99
-- All done.
