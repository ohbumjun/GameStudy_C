# 스토어드 함수 생성 권한
set global log_bin_trust_function_creators = 1;

# 가수 그룹의 평균 인원수 구하는 Stored Procedure
use market_db;

select * from member;

drop procedure if exists cursor_proc;
delimiter $$
create procedure cursor_proc()
begin
# -- 사용할 변수 준비
	declare memNumber int;
    declare cnt int default 0;
    declare totNumber int default 0;
    declare endOfRow boolean default false; # 행 끝 파악하기 위한 변수
# -- 커서 선언 : 이 경우 cursor 라는 것은 사실 select 문이다. 
#	즉, member 테비을 조회하는 구문을 커서로 만들기
    declare memberCursor cursor for
		select mem_number from member;

# -- 반복 조건 선언
	declare continue handler  # 반복 조건을 준비하는 예약어
		for not found # 더이상 행이 없을 때 이어진 문장을 수행 
        set endOfRow = true; # 행이 끝나면 endofRow 에 true 대입

# -- 커서 열기
    open memberCursor;
    
# -- 행 반복하기
    cursor_loop : LOOP # cursor_loop : 반복할 부분의 이름
		fetch memberCursor into memNumber; # fetch : 한행씩 읽어오기
											# memNumber 에는 각 회원 인원수가 하나씩 저장
        
        if endOfRow then
			leave cursor_loop; # leave : 반복할 이름을 빠져나감
		end if;
        
        set cnt = cnt + 1;
        set totNumber = totNumber + memNumber;
	end loop cursor_loop;
    
    select (totNumber/cnt) as '회원의 평균 인원 수'; # 반복문 빠져나오면 평균 인원수 계산
    
    close memberCursor; # 커서 닫기
    
end $$
delimiter ;

# stored procedure 결과 확인하기
call cursor_proc();