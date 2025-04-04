# import pkg_resources
# installed_packages = [pkg.key for pkg in pkg_resources.working_set]
# print('pymysql' in installed_packages)

#import sys
#print(sys.executable)

import pymysql

conn = pymysql.connect(host='127.0.0.1',
                       user = 'root',
                       password = '0000',
                       db = 'solodb',
                       charset = 'utf8') # db 연동

cur = conn.cursor(); # 커서 : DB 에 SQL 문을 실행하거나 실행된 결과를 돌려받는 통로

# 테이블 생성
cur.execute("create table userTable (id char(4), userName char(15), email char(20), birthYear int)")

cur.execute("insert into userTable values('hong', '홍지윤', 'hong@naver.com', '1996')")
cur.execute("insert into userTable values('kim', '김태연', 'kim@naver.com', '1996')")
cur.execute("insert into userTable values('star', '사랑', 'hong@naver.com', '1996')")

# 위의 입력 데이터를 db 에 완전히 저장하기. 위에까지 하면 임시 저장 상태. 이를 확실하게 저장하는 것을 '커밋' 이라고 한다.
conn.commit() 

# db 닫기
conn.close()


