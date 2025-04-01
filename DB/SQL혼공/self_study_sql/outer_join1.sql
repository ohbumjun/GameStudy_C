use market_db;
-- 구매 기록이 없는 회원정보도 함께
select M.mem_id, M.mem_name, B.prod_name
	from member M
		left outer join buy B -- left outer join : 왼쪽 테이블은 모두 출력되어야 한다.
        on M.mem_id = B.mem_id
        order by M.mem_id;
        
-- 가입만 하고, 구매 기록은 없는 회원'만'
select * from buy;
select * from member;

select M.mem_id, M.mem_name, B.prod_name
	from member M
    left outer join buy B
    on M.mem_id = B.mem_id
    where B.prod_name is NULL;
    