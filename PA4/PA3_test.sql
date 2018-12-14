--CS457 PA3

--Construct the database and table (0 points; expected to work from PA1)
CREATE DATABASE CS457_PA3;
USE CS457_PA3;
create table Employee(id int, name varchar(10));
create table Sales(employeeID int, productID int);
<<<<<<< HEAD
=======
create table Bepis(productName varchar(10), productID int);

>>>>>>> 86461356e04e7b431fc415b8a87dd5a17aada52d

--Insert new data (0 points; expected to work from PA2)
insert into Employee values(1,'Joe');
insert into Employee values(2,'Jack');
insert into Employee values(3,'Gill');
insert into Sales values(1,344);
insert into Sales values(1,355);
insert into Sales values(2,544);
<<<<<<< HEAD
=======
insert into Bepis values('bepCream',344);
insert into Bepis values('peanuts',355);
insert into Bepis values('bepis',544);
insert into Bepis values('wed',420);
>>>>>>> 86461356e04e7b431fc415b8a87dd5a17aada52d

-- The following will miss Gill (20 points)
select * 
from Employee E, Sales S 
where E.id = S.employeeID;

-- This is the same as above but with a different syntax (20 points)
select * 
from Employee E inner join Sales S 
on E.id = S.employeeID;

-- The following will include Gill (30 points)
select * 
from Employee E left outer join Sales S 
on E.id = S.employeeID;

<<<<<<< HEAD
=======
select * 
from Bepis B left outer join Sales S 
on B.productID = S.productID;

>>>>>>> 86461356e04e7b431fc415b8a87dd5a17aada52d
-- Expected output
--
-- Database CS457_PA3 created.
-- Using database CS457_PA3.
-- Table Employee created.
-- Table Sales created.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- 1 new record inserted.
-- id int|name varchar(10)|employeeID int|productID int
-- 1|Joe|1|344
-- 1|Joe|1|355
-- 2|Jack|2|544
-- id int|name varchar(10)|employeeID int|productID int
-- 1|Joe|1|344
-- 1|Joe|1|355
-- 2|Jack|2|544
-- id int|name varchar(10)|employeeID int|productID int
-- 1|Joe|1|344
-- 1|Joe|1|355
-- 2|Jack|2|544
-- 3|Gill||
