use market_db;

create table hongong1 (toy_id INT, toy_name char(4), age int);

insert into hongong1 values(1, '우디', 25);

insert into hongong1 (toy_name, age, toy_id) values ('제시', 1, 3);

create table hongong2 (
	toy_id int auto_increment primary key,
    toy_name char(4),
    age int);
    
insert into hongong2 values(NULL,'ㅁ', 1);
insert into hongong2 values (NULL,'ㄴ', 2);

select * from hongong2;

alter table hongong2 auto_increment = 100;
insert into hongong2 values (NULL,'t', 2);

create table hh3
	(toy_id int auto_increment primary key,
    toy_name char(4),
    age int);
alter table hh3 auto_increment = 100;
set @@auto_increment_increment = 3;

insert into hh3 values (NULL,'t', 2);
insert into hh3 values (NULL,'', 2);

select * from hh3;

create database world;

use world;

create table city world;