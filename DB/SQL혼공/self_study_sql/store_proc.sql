use market_db;

-- ifProc1
drop procedure if exists ifProc1;

DELIMITER  $$ -- stored procedure 의 시작 
create procedure ifProc1()
begin -- begin, end 사이에 sql 프로그래밍 코딩
	declare myNum int;
    set myNum = 200;
    if myNum = 100 then
		select '100 입니다';
	else
		select '100 아닙니다';
	end if;
end $$ -- stored procedure 의 종료
DELIMITER ; -- 종료 문자를 다시 세미콜론으로 변경

call ifProc1();


-- ifProc2
drop procedure if exists ifProc2;

DELIMITER $$
create procedure ifProc2()
begin
	declare debutDate DATE; -- 데뷔 일자
    declare curDate DATE; -- 오늘 
    declare days INT; 
    
    select debutDate into debutDate
    from market_db.member
    where mem_id = 'APN';
    
    set curDate = CURRENT_DATE(); -- 현재 날짜
    set days = DATEDIFF(curDate, debutDate); -- 날짜의 차이
    
    if (days / 365) >= 5 then -- 5년이 지났다면
		select concat('데뷔 : ', days);
	else
		select '데뷔 : ' + days + 'only';
	end if;
end $$
DELIMITER;
call ifProc3();