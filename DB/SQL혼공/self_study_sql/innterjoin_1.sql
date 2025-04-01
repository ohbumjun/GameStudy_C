use market_db;

select * from buy;

select * from member;

select * 
	from buy
    inner join member
    on buy.mem_id = member.mem_id
    where buy.mem_id = 'GRL'; 
    
select buy.mem_id, mem_name, prod_name, addr, concat(phone1, phone2) '연락처'
	from buy
    inner join member
    on buy.mem_id = member.mem_id;
    
-- 별칭 버전
select M.mem_id, M.mem_name, B.prod_name, M.addr, concat(phone1, phone2) '연락처'
	from buy B
    inner join member M
    on B.mem_id = M.mem_id
    order by M.mem_id;
    
-- 중복 이름 필요 x, 우리 사이트에서 한번이라도 구매한 기록이 있는 회원들만 추출
select distinct M.mem_id, M.mem_name
	from buy B
    inner join member M
    on B.mem_id = M.mem_id
    order by M.mem_id;