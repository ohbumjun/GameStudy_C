use market_db;
-- 인터넷 db 회원들은 물건을 구매한다
-- 회원들의 총 구매액을 계산해서 회원의 등급을 4단계로 구분하려고 한다

-- buy 에서 회원별 총 구매액 구하기
select * from buy;
select * from member;

select mem_id,  sum(price * amount)
	from buy
	group by mem_id
    order by sum(price * amount) desc;
    
-- 회원 이름도 조회하기

select B.mem_id, M.mem_name, price 
	from buy B
    inner join member M
    on B.mem_id = M.mem_id;
    
select *
	from buy B
    inner join member M
    on B.mem_id = M.mem_id;
    
select B.mem_id, M.mem_name, sum(B.price * B.amount)
	from buy B
    inner join member M
    on B.mem_id = M.mem_id
    group by B.mem_id;
    
-- 구매하지 않은 회원 정보도 추출하기
select M.mem_id, M.mem_name, sum(B.price * B.amount)
	from buy B
    right outer join member M
    on B.mem_id = M.mem_id
    group by M.mem_id -- B 가 아니라 M 으로 변경. right outer join 을 통해 M 기준으로 join
    order by sum(B.price * B.amount) desc;

-- CASE 문
select M.mem_id, M.mem_name, sum(B.price * B.amount) '회원등급', 
	case -- 새로운 열 추가. 총 구매액에 따라 회원 분류
		when (sum(B.price * B.amount) >= 1500) then '최우수고객'
        when (sum(B.price * B.amount) >= 1000) then '우수고객'
        when (sum(B.price * B.amount) >= 500) then '일반고객'
        else '유령고객'
    end "회원등급"
	from buy B
    right outer join member M
    on B.mem_id = M.mem_id
    group by M.mem_id -- B 가 아니라 M 으로 변경. right outer join 을 통해 M 기준으로 join
    order by sum(B.price * B.amount) desc;