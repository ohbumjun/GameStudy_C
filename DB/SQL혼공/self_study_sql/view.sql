-- view

use market_db;
create view v_member
as
	select mem_id, mem_name, addr from member; -- view 의 실체는 select

select * from v_member;

-- view 생성, 기존 테이블 이름과 다른 이름의 열 생성
create view v_viewtest1
as
	select B.mem_id 'Member ID', M.mem_name AS 'Member Name', -- 별칭 'AS' 사용
		B.prod_name 'Product Name',
					concat(M.phone1, M.phone2) AS 'Office Phone'
		from buy B
			inner join member M
            on B.mem_id = M.mem_id;

select * from v_viewtest1;

-- 중요 : 뷰 조회시 열 이름에 공백이 있으면 `` 백틱으로 묶어줘야 한다.
select distinct `Member ID`, `Member Name` from v_viewtest1;

drop view v_viewtest1;

-- create or replace view : create 은 기존에 있으면 에러, replace_new 의 경우 기존에 있으면 덮어쓰기. 따라서 에러 x
create or replace view v_viewtest2
as
	select mem_id, mem_name, addr from member;
    
describe v_viewtest2; -- view 는 primary key 등의 정보는 확인 x
describe member;

-- 소스 코드 보기
show create view v_viewtest2;

-- 뷰를 통한 데이터 수정 / 삭제
select * from v_member;
update v_member set addr = '부산' where mem_id = 'BLK';

select * from member;

-- 에러 : mem_number 는 not null 이기 때문이다
-- 해결방법 : v_member 라는 뷰가 mem_number 열을 포함하도록 하기 / mem_number 열의 속성을 not null 이 아닌 null 도 허용하게 하기
insert into v_member(mem_id, mem_name, addr) values ('BTS', '방탄소년단', '경기');

create view v_height167
as
	select * from member where height >= 167;

select * from v_height167;

insert into v_height167 values ('TRA', '티아라', 6, '서울', NULL, NULL, 159, '2005-01-01');

-- with check option : 뷰에 설정된 값의 범위가 벗어나는 값은 입력되지 않게 하는 옵션
alter view v_height167
as
	select * from member where height >= 167
		with check option ;
        
insert into v_height167 values ('sss', '에스에스', 7, '서울', NULL, NULL, 150, '2005-01-01');
