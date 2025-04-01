SET SQL_SAFE_UPDATES = 0;

USE market_db;

CREATE TABLE emp_table (
    emp CHAR(4),
    manager CHAR(4),
    phone VARCHAR(8)
);

delete from emp_table where emp = '대표';

INSERT INTO emp_table VALUES ('대표', NULL, '0000');
INSERT INTO emp_table VALUES ('영업이사', '대표', '1111');
INSERT INTO emp_table VALUES ('관리이사', '대표', '2222');
INSERT INTO emp_table VALUES ('정보이사', '대표', '3333');
INSERT INTO emp_table VALUES ('영업과장', '영업이사', '1111-1');
INSERT INTO emp_table VALUES ('경리부장', '관리이사', '2222-1');
INSERT INTO emp_table VALUES ('인사부장', '관리이사', '2222-2');
INSERT INTO emp_table VALUES ('개발팀장', '정보이사', '3333-1');
INSERT INTO emp_table VALUES ('개발주임', '정보이사', '3333-1-1');

select * from emp_table;

-- 경리 부장 직속 상관의 연락처 알기
select A.emp '직원', B.emp '직속상관', B.phone '상관 연락처'
	from emp_table A
		inner join emp_table B
        on A.manager = B.emp
	where A.emp = '경리부장';

