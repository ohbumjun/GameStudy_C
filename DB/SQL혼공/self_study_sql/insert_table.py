import pymysql

conn, cur = None, None;
data1, data2, data3, data4 = "", "", "", ""
sql = ""

conn = pymysql.connect(host = '127.0.0.1', user = 'root', 
                       password = '0000', db = 'solodb', charset= 'utf8')
cur = conn.cursor()

while(True) :
    data1 = input("사용자 ID =>a ")
    if data1 == "" :
        break;
    data2 = input("사용자 이름 : ")
    data3 = input("사용자 이메일 : ")
    data4 = input("사용자 출생연도: ")
    sql = "insert into userTable values('" + data1 + "','" + data2 + "', '" + data3 + "', '" + data4 + "')"
    cur.execute(sql)

conn.commit()
conn.close()