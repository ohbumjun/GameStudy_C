/*
항목 8. 예외가 소멸자를 떠나지 못하도록 붙들어 놓자   -------------------
*/

// C++은 예외를 내보내는 소멸자를 좋아하지 않는다
class DBConnection
{
public :
    static DBConnection create();
    void close();
};

class DBConn // DBConnection 객체를 관리하는 클래스 
{
public :
    DBConn(DBConnection create());
    ~DBConn() // 데이터 베이스 연결이 항상 닫히도록 챙겨주는 함수 
    {
        // db.close() 함수를 호출했는데, 여기서 예외가 발생한다면
        // DBConn의 소멸자는, 분명히 이 예외를 전파할 것이다
        // 즉, 그 소멸자에서 예외가 나가도록 내버려둔다는 것이다
        // 예외를 던지는 소멸자는 곧 '걱정거리'
        db.close();
    }
private :
    DBConnection db;
};


// 프로그램 끝내거나, 삼키기
~DBConn() 
{
    try{db.close();}
    catch(...)
    {
        // close 호출 실패 로그 작성
        std::abort();
    }
}

~DBConn() 
{
    try{db.close();}
    catch(...)
    {
        // close 호출 실패 로그 작성 --> 삼키기 
    }
}


// DBConn에서 close 함수를 직접 제공하게 되면
// 이 함수의 실행 중에 발생하는 예외를 사용자가 직접 처리할 수 있을 것이다.
// DBConnection이 닫혔는지의 여부를 유지했다가 
// 닫히지 않았으면 DBConn의 소멸자에서 닫을 수도 있을 것이다
// 이렇게 하면 데이터베이스 연결이 누출되지 않는다
class DBConn
{
public :
    ~DBConn()
    {
        if(!closed)
        {
            // 사용자가 연결을 안닫았으면
            // 여기서 닫아 본다. 
            try
            {
                db.close();
            }
            catch(...)
            {
                // close 호출이 실패했다는 로그를 작성한다. 
                // 즉, 실패를 알리 후에, 
                // - 실행을 끌내거나 abort
                // - 예외를 삼킨다. ==> (참고) 예외를 그냥 무시한 뒤라도, 프로그램이 신뢰성 있게 실행을 지속할 수 있게 해줘야 한다. 
            }
        }
    }
    void close()
    {
        db.close();
        closed = true;
    }
private :
    DBConnection db;
    bool closed;
};

// close 호출의 책임을 DBConn의 소멸자에서 DBConn의 사용자로 떠넘기는 것이다. 
// 핵심은, 어떤 동작이 예외를 일으키면서, 실패할 가능성이 있고,
// 또 그 예외를 처리해야할 필요가 있다면
// 그 예외는, "소멸자가 아닌 다른 함수"에서 비롯된 것이어야 한다.는 것이다.
// 왜냐 ?
// 예외를 일으키는 소멸자는 시한폭탄이나 마찬가지라서, 프로그램의 불완전 종료 혹은
// 미정의 동작의 위험을 내포하기 때문이다.

// 위의 코드를 보면, 사용자에게 에러를 처리할 수 있는 기회를 주는 것이다
// 이것마저 없다면, 사용자는 예외에 대처할 기회를 못잡게 된다.

// 정리 
//- 소멸자에서는, 예외가 빠져나가서는 안된다. 만약 소멸자 안에서 호출된 함수가 예외를 던질 가능성이 있다면
// - 어떤 예외이든지 소멸자에서 모두 받아낸 후에, 삼켜 버리든지 프로그램을 끝내든지 해야 한다.
// - 어떤 클래스의 연산이 진행되다가 던진 예외에 대해 사용자가 반응해야 할 필요가 있다면
// 해당 연산을 제공하는 함수는 반드시 소멸자가 아니라, 보통의 함수이어야 한다. 
