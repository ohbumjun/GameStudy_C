use market_db;

delete from city_popul 
	where city_name like 'New%'; -- New 로 시작하는 도시 모두 지우기
    
delete from city_popul 
	where city_name like 'New%'
    limit 3; -- New 로 시작하는 도시, 3개만 지우기
    
select * from city_popul;