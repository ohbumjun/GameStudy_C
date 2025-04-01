use market_db;

SET SQL_SAFE_UPDATES = 0;

create table city_popul (city_name CHAR(35), population INT);

insert into city_popul values ('Seoul', '1');
insert into city_popul values ('NewYork', '3');

select * from city_popul;

update city_popul 
	set city_name = '서울'
    where city_name = 'Seoul';
    
update city_popul 
	set population = population / 1000;

    
SET SQL_SAFE_UPDATES = 1; -- Re-enable safe mode (optional)