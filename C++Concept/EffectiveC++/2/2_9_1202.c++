// 항목 9 : 객체 생성 및 소멸 과정 중에는 절대로 가상 함수를 호출하지 말자

// 모든 거래에 대한 기본 클래스 
class Transaction
{
public :
    Transaction() // 기본 클래스 생성자 
    {
        logTransaction();
    }
    // 타입에 따라 달라지는 로그 기록을 만듭니다 
    virtual void logTransaction() const = 0;
};

class BuyTransaction : public Transaction
{
public :
    virtual void logTransaction() const ;
};

BuyTransaction b;

/*
BuyTransaction 생성자가 호출되는 것은 맞다.
- 단, 파생 클래스 객체가 생성될 때, 그 객체의 기본 클래스 부분이
- 파생 클래스 부분보다 먼저 호출되는 것이 정석이다
따라서, Transaction 생성자가 먼저 호출될 것이다.
---
Transaction 생성자의 마지막 줄에서
가상 함수 logTransaction()을 호출한다. 
중요한 것은 logTransaction 은 BuyTransaction 이 아니라, Transaction 것이다.
왜 ?
기본 클래스의 생성자가 호출되는 동안
가상 함수는 절대로 파생 클래스 쪽으로 내려가지 않는다
대신, 객체 자신이 기본 클래스 타입인 것처럼 동작한다.
기본 클래스 생성 과정에서는, 가상 함수가 먹히지 않는다는 것이다.
---
기본 클래스 생성자는 파생 클래스 생성자보자 앞서서 실행된다.
기본 클래스 생성자가 돌아가고 있을 시점에 
파생 클래스 데이터 멤버는 아직 초기화된 상태가 아니라는 것이다. 
파생 클래스 버전의 가상 함수는, 
파생 클래스만의 데이터 멤버를 건드릴 것인데, 
이들은 아직 초기화 되지 않았다는 것이다. 
---
파생 클래스 객체의 기본 클래스 부분이 생성되는 동안은
그 객체의 타입은, 바로 기본 클래스이다.
호출되는 가상 함수는 모두 기본 클래스의 것으로 결정된다.
런타임 타입 정보를 사용하는 typeid 등도, 모두 기본 클래스 타입의 객체로 취급한다.

BuyTransaction 객체의 기본 클래스 부분을 초기화 하기 위해
Transaction 생성자가 실행되고 있는 동안에는
그 객체의 타입이 Transaction 이라는 것이다.
BuyTransaction 클래스만의 데이터는 아직 초기화된 상태가 아니기 때문에
아예 없었던 것처럼 취급하는 편이 안전하다는 것이다. 
파생 클래스의 생성자으 실행이 시작되어야만
그 객체가 비로소 파생 클래스 객체의 면모를 갖게 된다.
---
소멸자도 마찬가지이다
파생 클래스 소멸자가 호출되고 나면, 파생 클래스만의 데이터 멤버는
정의되지 않은 값으로 가정하기 때문에
이제부터 C++는 이들을 없는 것처럼 취급하고 진행한다.
기본 클래스 소멸자에 진입할 당시 객체는 기본 클래스 객체가 된다. 

*/

// 자. 해결책은 무엇일가
// Transaction 생성자가 여러개 된다. (기본 + 파생)
// 각자가 하는 일 중에서, 같은 작업은 모아서 공동의 초기화 코드로 만들어두며
// 코드 판박이 현상을 막을 수 있을 것이다
// 이를 위한 해결책은, 공동 초기화 코드를, 기본 클래스, 멤버 함수로 세팅하는 것이다
class Transaction
{
public :
    explicit Transaction(const std::string& Info)
    {
        logTransaction(Info); // 비가상함수 호출 
    }
    void logTransaction(const std::string& logInfo) const ; // 비가상 함수 
};

class createLogString : public Transaction
{
public :
    BuyTransaction(parameters) : Transaction(createLogString(parameters)){} // 로그 정보를 기본 클래스 생성자로 넘긴다
private :
    // 정정 멤버 함수
    static std::string createLogString(parameters);
};

// 기본 클래스 부분이 생성될 때는, 가상 함수를 호출한다고 해도
// 기본 클래스의 울타리를 넘어 내려갈 수 없기 때문에
// 필요한 초기화 정보를, 파생 클래스 쪽에서 기본 클래스 생서자로 '올려' 주도록 만듦으로써
// 부조간 부분을 역을 채울 수 있다는 것이다. 

// createLogString == 정적 함수
// 정적 멤버로 되어 있기 때문에, 정적 멤버 변수만 건드릴 수 있다
// 따라서, 생성이 채 끝나지 않은 BuyTransaction 객체으 미초기화된 데이터 멤버를
// 자칫 실수로 건드릴 위험도 없다. 
// 왜냐햐면, "미초기화된 데이터 멤버는" 정의 되지 않은 상태에 있다.라는 사실 때문이다
// 이로 인해, 기본 클래스 부분의 생성과 소멸이 진행되는 동안에 호출되는 가상 함수가 
// 무턱대로, 파생 클래스 쪽으로 내려가지 않는 것이다 

