SET SQL_SAFE_UPDATES = 0;
use market_db;

create table if not exists trigger_table (id int, txt varchar(10));
insert into trigger_table values(1, '레드벨벳');
insert into trigger_table values(2, '잇지');
insert into trigger_table values(3, '블랙핑크');

# after trigger
drop trigger if exists myTrigger;

# delete trigger
delimiter $$
create trigger myTrigger
	after delete -- delete 문 발생 이후 작동하라. 라는 의미
    on trigger_table -- 트리커 부착할 테이블 지정
    for each row -- 각 행마다 적용 (항상 써주는 구문)
begin
	set @msg = '가수 삭제'; -- 트리거 실행시 작동되는 코드들
end $$
delimiter ;

select * from trigger_table;
delete from trigger_table where id = 3;
select @msg;

drop table if exists singer;
create table singer (select mem_id, mem_name, mem_number, addr from member);
select * from singer;

# insert, update 일어나는 경우, 변경 이전 데이터를 저장할 백업 테이블 생성
create table backup_singer
(
	mem_id char(8) not null,
    mem_name varchar(10) not null,
    mem_number int not null,
    addr char(2) not null,
    modType char(2), -- 변경된 타입 ex) insert, update
    modDate DATE,    -- 변경 날짜
    modUser varchar(30) -- 변경 사용자
);

# update trigger
drop trigger if exists singer_updateTrg;
delimiter $$
create trigger singer_updateTrg
	after update -- update 이후 실행
    on singer
    for each row
begin
# OLD table : update, delete 등이 수행될 때, 변경되기 전의 데이터가 잠깐 저장되는 임시 테이블
#   old table 에 update 문이 작동되면 
#	이 행에 의해서 업데이트 되기 전의 데이터가 백업테이블(backup_singer) 에 입력됨. 
#	즉, 원본 데이터 보존
	insert into backup_singer values(OLD.mem_id,
		OLD.mem_name,
        OLD.mem_number,
        OLD.addr,
        '수정',
        CURDATE(),
        CURRENT_USER()
	);
end $$
delimiter ;

update singer set addr = '영국' where mem_id = 'BLK';
select * from backup_singer;

# delete trigger
drop trigger if exists singer_deleteTrg;
delimiter $$
create trigger singer_deleteTrg
	after delete
    on singer
    for each row
begin
	insert into backup_singer values(
		OLD.mem_id,
        OLd.mem_name,
        OLD.mem_number,
        OLD.addr,
        '삭제',
        CURDATE(),
        current_user()
	);
end $$
delimiter ;

delete from singer where mem_number >= 7;
select * from backup_singer;

