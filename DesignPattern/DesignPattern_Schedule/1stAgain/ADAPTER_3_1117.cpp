/*
[ 시나리오 ]
오리와 칠면조 클래스가 있다. 
칠면조를 오리로 둔갑 시켜야 한다. 
그러나 이 두 클래스는 서로의 인터패이스가 틀리다는 문제가 있다.

[ 요약 ]
오리 클래스를 상속받고 칠면조 클래스는 레어링 시킨다. 
그리고 오리 클래스 인터페이스에 맞쳐서 연결 시키면 된다.

*/

#include <iostream>
#include <string>

using namespace std;

// Target Interface
class Duck
{
public:
    virtual void Fly(void) = 0;
    virtual void Quack(void) = 0;
    virtual ~Duck(){}
};

class MallardDuck : public Duck
{
public:
    virtual void Fly(void) { cout << "청동오리인 저는 날고 있어요" << endl; }
    virtual void Quack() { cout << "꽥" << endl; }
};

// Adaptee interface
class Turkey
{
public:
    virtual void Fly() = 0;
    virtual void Gobble() = 0;
    virtual ~Turkey(){}
};

class WildTurkey : public Turkey
{
public:
    virtual void Fly() { cout << "칠면조인 저는 조금 날아요" << endl; }
    virtual void Gobble() { cout << "골골" << endl; }
};

class TurkeyAdapter : public Duck
{
private:
    Turkey *m_pTurkey;

public:
    TurkeyAdapter(Turkey *pTurkey)
    {
        m_pTurkey = pTurkey;
    }

public:
    virtual void Fly()
    {
        cout << "원래 오리인데, 칠면조로 위장중" << endl;
        m_pTurkey->Fly();
    }
    virtual void Quack() { m_pTurkey->Gobble(); }
};

int main()
{
    Duck *pMallardDuck = new MallardDuck;
    pMallardDuck->Fly();
    pMallardDuck->Quack();

    Turkey *pWildTurkey = new WildTurkey;
    pWildTurkey->Fly();
    pWildTurkey->Gobble();

    // TurkeyAdapter *pTurkeyAdapter = new TurkeyAdapter(pWildTurkey);
    Duck *pTurkeyAdapter = new TurkeyAdapter(pWildTurkey); // Client는 Target Interface만을 바라본다고 했으므로
    pTurkeyAdapter->Fly();
    pTurkeyAdapter->Quack();

    return 0;
}