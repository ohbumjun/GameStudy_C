use market_db;

drop table if exists buy, member;

create table member
(
	mem_id char(8) not null primary key,
    mem_name varchar(10) not null,
    height tinyint unsigned null
);

create table buy
(
	num int auto_increment not null primary key,
    mem_id char(8) not null,
    prod_name char(6) not null
    -- foreign key(mem_id) references member(mem_id) -- 외래키 지정 방법
);


drop table if exists buy;

create table buy
(
	num int auto_increment not null primary key,
    mem_id char(8) not null,
    prod_name char(8) not null
);

-- 외래 키 조건 추가하기
-- alter table buy
--	add constraint
--    foreign key(mem_id)
--     references member(mem_id);
    
insert into member values('BLK', '핑크', 163);
insert into buy values(null, 'BLK', '지갑');
insert into buy values(null, 'BLK', '맥북');

select * from buy
	inner join member
    on buy.mem_id = member.mem_id;
    
-- 기준 테이블 column 값 변경
-- 외래키로 맺어진 이후에는 기준 테이블 열 이름 변경 x
update member set mem_id = 'PINK' where mem_id = 'BLK';

-- 변경될 수 있도록 지원 : on update, on delete cascade
alter table buy
	add constraint
    foreign key(mem_id) references member(mem_id)
    on update cascade
    on delete cascade;

update member set mem_id = 'PINK' where mem_id = 'BLK';
delete from member where mem_id = 'PINK';

-- 기본값 정의
create table member
(
	mem_id char(8) not null primary key,
    mem_name varchar(10) not null,
    height tinyint unsigned null
);



