# 스토어드 함수 생성 권한
set global log_bin_trust_function_creators = 1;

use market_db;

drop function if exists sumFunc;

delimiter $$
create function sumFunc(number1 int, number2 int)
returns int
begin
    return number1 + number2;
end $$
delimiter ;

select sumFunc(100, 200) as '합계';

# 함수 내역 확인
# show create function 함수_이름;
show create function sumFunc;

drop function sumFunc;