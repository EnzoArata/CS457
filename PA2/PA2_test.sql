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
insert into bepis values(2,	20, 11,       24);

select * from Product;

select * from bepis;

--Modify data (20 points)
update Product 
set name = 'Gizmo' 
where name = 'SuperGizmo';

update Product 
set price = 14.99 
where name = 'Gizmo';

select * from Product;

--Delete data (20 points)
delete from Product 
where name = 'PowerGizmo';

delete from Product 
where name = 'Gizmo';

delete from Product 
where price > 1;

delete from bepis
where height > 8;

select * from Product;

select * from bepis;

--Query subsets (10 points)
select name, price 
from product 
where pid != 2;

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
