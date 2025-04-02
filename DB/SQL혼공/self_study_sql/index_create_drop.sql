use market_db;

select * from member;

show index from member; # Primary : 클러스터형 인덱스

show table status;

create index idx_member_addr on member(addr); # 중복 허용 보조 인덱스
show index from member; 

analyze table member;
show index from member;

create unique index idx_member_mem_number # 에러. 중복 값 이미 존재
on member(mem_number);

create unique index idx_member_mem_name
on member(mem_name);

analyze table member; # 지금까지 만든 인덱스 모두 적용
show index from member;

select * from member;

select mem_name, addr from member;

select mem_name, addr from member
where mem_name = '에이핑크';

create index idx_member_mem_number
on member(mem_number); # 중복 허용. 숫자 범위 조회

analyze table member;

select mem_name, mem_number from member where mem_number >= 7;

select mem_name, mem_number from member where mem_number >= 1;

select mem_name, mem_number from member where mem_number * 2 >= 14 / 2;

drop index idx_member_mem_name on member;
drop index idx_member_addr on member;
drop index idx_member_mem_number on member;

alter table member drop primary key; # Primary Key 설정된 인덱스 : 외래키 지정있어서 에러

select table_name, constraint_name # buy 측에 설정된 외래키 보기 
from information_schema.referential_constraints
where constraint_schema = 'market_db';

alter table buy drop foreign key buy_ibfk_1; # 외래키 제거

alter table member drop primary key; # 클러스터형 인덱스 제거

 



