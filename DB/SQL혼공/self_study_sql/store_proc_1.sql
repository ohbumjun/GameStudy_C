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

