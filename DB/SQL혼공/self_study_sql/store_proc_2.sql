select * from market_db.member;

-- ifProc2
drop procedure if exists ifProc2;

DELIMITER $$

create procedure ifProc2()
begin
	declare debutDate DATE; -- 데뷔 일자
    declare curDate DATE; -- 오늘 
    declare days INT; 
    
    select debut_date into debutDate -- 데뷔일자 추출. INTO 변수 : 결과를 변수 저장하기
    from market_db.member
    where mem_id = 'APN';
    
    set curDate = CURRENT_DATE(); -- 현재 날짜
    set days = DATEDIFF(curDate, debutDate); -- 날짜의 차이
    
    select days / 365;
    
    if (days / 365) >= 5 then -- 5년이 지났다면
		select concat('데뷔 : ', days, '이나 지났음 !');
	else
		-- select '데뷔 : ' + days + 'only';
		select days;
	end if;
end $$

DELIMITER ;

call ifProc2();