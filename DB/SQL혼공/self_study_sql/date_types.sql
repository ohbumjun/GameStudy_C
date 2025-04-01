use market_db;

-- Int --
create table hg4 (
	tinyint_col tinyint,
    bigint_col BIGINT
);

insert into hg4 values (127, 900000000000000);

create table ushg4 (
	tinyint_col tinyint unsigned, -- 원래 값 범위 : -127 ~ 127, 하지만 unsigned 로 인해서 0 이상의 값만 insert
    bigint_col BIGINT
);

insert into ushg4 values (=127, 900000000000000); -- 에러

-- CHAR --
-- char : 고정길이 문자형 		ex) char(10) 에서 3개만 쓰면 7개 메모리는 낭비 / 성능은 더 좋음 / 범위 최대 255
-- varchar : 가변길이 문자형 	ex) varchar(10) 에서 3개만 쓰면 3개만 사용. 낭비 x / 하지만 char 에 비해 속도 저하 / 번위 최대 16383 

-- 변수 사용 --
set @myVar1 = 5;
set @myVar2 = 5;
select @myVar1;
select @myVar2 + @myVar1;

set @txt = '가수이름 ==> ';
set @heightH = 166;
set @count = 3;
select * from member;
-- select @txt, mem_name from member where height > @heightH limit @count;
prepare mySql from 'select @txt, mem_name from member where height > @heightH limit ?';
execute mySql using @count;

-- 데이터 변환 : case, concat 함수 사용
select avg(price) as '평균 가격' from buy;
select cast(avg(price) as signed) as '평균 가격' from buy; -- 부호 있는 정수
select cast(avg(price) as unsigned) as '평균 가격' from buy; -- 부호 없는 정수

select num , concat(cast(price as char), 'X', cast(amount as char), '=') -- concat : 문자를 이어주는 역할 => 30 x 2 = 와 같은 형태의 문자를 만들어서 출력
	'가격x수량', price * amount '구매액'
    from buy;

select concat(100, '200'); -- 숫자 100 이 문자 '100' 으로 변환
select 100 + '200'; -- 문자가 숫자 200 으로 변환 ex) 암묵적 변환

