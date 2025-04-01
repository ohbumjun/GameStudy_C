-- use market_db; -- 앞으로 모든 sql 문은 market db 에서 수행된다.
select * from market_db.member;
select mem_name, addr from member where mem_name = '블랙핑크';

select mem_name, addr from member where height > 165 AND addr = '경기';

select mem_name, addr from member where height > 165 AND height < 168;

select mem_name, addr from member where height BETWeen 165 AND 168;

select mem_name, addr from member where addr in ('경기', '서울');

select mem_name, addr from member where mem_name LIKE '__핑크';

select mem_name from member where height > (select height from member where mem_name = '에이핑크');

select height from member where mem_name = '에이핑크';

select * from member order by debut_date;

select * from member where mem_number > 5 order by debut_date desc, height;

select * from member limit 3;

select * from buy ;

select addr from member order by addr;

select distinct addr from member order by addr ;

select mem_id "회원 아이디", SUM(amount) "총 구매 개수" from buy group by mem_id;

select mem_id "회원 아이디", SUM(amount * price) "총 구매 총합" from buy group by mem_id;

select AVG(amount) from buy group by mem_id;

select count(*) from buy;

select * from member;

select count(phone1) from member;

select mem_id "회원 아이디", sum(price * amount) "총 구매 금액" from buy 
	group by mem_id
    having sum(price * amount) > 1000
    order by sum(price * amount) desc;

