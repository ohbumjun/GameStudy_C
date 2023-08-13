// state 패턴
// 객체의 상태를 동적으로 바꿔보자
using namespace std;

// 상태, state에 해당하는 interface state class를
// 별도로 정의한다
class IState
{
public:
    virtual void run() = 0;
    virtual void attack() = 0;
}

class Character
{
    IState *p;

public:
    void setState(IState *cp) { p = cp; }
    void run() { p->run(); }
    void attack() { p->attack(); }
}

class NormalState : public IState
{
    void run() { cout << "run" << endl; }
    void attack() { cout << "attack" << endl; }
}

class FastState : public IState
{
    void run() { cout << "fast run" << endl; }
    void attack() { cout << "fast attack" << endl; }
}

int
main()
{
    NormalState ns;
    FastState fs;
    Character *p = new Character; //
    p->setState(&ns);
    p->run();
    p->attack();

    p->setState(&fs);
    p->run();
    p->attack();
}