
drop procedure if exists whileProc1;

DELIMITER $$

create procedure whileProc1()
begin
	declare i int; 
    declare hap int;
    set i = 1;
    set hap = 0;
    
    myWhile : -- while 문을 myWhild 이라는 레이블로 지정
    while(i <= 100) do
		if (i%4 = 0) then
			set i = i + 1;
            iterate myWhile; -- 지정한 label 문으로 가서 진행 일종의 goto ?
		end if;
        set hap = hap + i;
        if (hap > 1000) then
			leave myWhile; -- while 종료, break
		end if;
        set i = i + 1;
	end while;
    
    select hap;
    
end $$

DELIMITER ;

call whileProc1();