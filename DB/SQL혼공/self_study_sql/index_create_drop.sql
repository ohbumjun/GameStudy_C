use market_db;

select * from member;

show index from member; # Primary : 클러스터형 인덱스

show table status;

create index idx_member_addr on member(addr); # 중복 허용 보조 인덱스
show index from member; 

analyze table member;
show index from member;

create unique index idx_member_mem_number
on member(mem_number);

create unique index idx_member_mem_name
on member(mem_name);

analyze table member; # 지금까지 만든 인덱스 모두 적용

