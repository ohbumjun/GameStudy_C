import pymysql

con, cur = None, None;
data1, data2, data3, data4 = "", "", "", ""
row = None

conn = pymysql.connect(host='127.0.0.1',
                       user = 'root',
                       password = '0000',
                       db = 'solodb',
                       charset = 'utf8') # db 연동

cur = conn.cursor(); # 커서 : DB 에 SQL 문을 실행하거나 실행된 결과를 돌려받는 통로

# 테이블 생성
cur.execute("select * from userTable")

while(True) :
    row = cur.fetchone()
    if row == None :
        break 
    data1 = row[0]
    data2 = row[1]
    data3 = row[2]
    data4 = row[3]
    print ("%s, %s, %s, %d" % (data1, data2, data3, data4))
conn.close()
