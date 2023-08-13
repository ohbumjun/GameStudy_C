// 데코레이터 패턴
// 객체의 기능을 추가할 수 있는 패턴

// 1) 상속을 통한 기능의 추가
// 2) 포함을 사용한 기능의 추가
// 3) 인터페이스 추가하기
#include <iostream>

using namespace std;

// 3) Interface 추가하기
struct IComponent
{
    virtual void Fire() = 0;
    virtual ~IComponent(){};
};

class I_SpaceShip : public IComponent
{
public:
    int color;
    void Fire() { cout << "Fire Missile" << endl; }
};

class MiddleMissile : public IComponent
{
    IComponent *ss;

public:
    MiddleMissile(IComponent *p) : ss(p) {}
    void Fire()
    {
        ss->Fire();
        cout << "Fire Left Missile" << endl;
    }
};

class DownMissile : public IComponent
{
    IComponent *ss;

public:
    DownMissile(IComponent *p) : ss(p) {}
    void Fire()
    {
        ss->Fire();
        cout << "Fire Down Missile" << endl;
    }
};

// ---------------------------------------------------------

class SpaceShip
{
public:
    int color;
    void Fire() { cout << "Fire Missile" << endl; }
};

// 1) 상속을 사용한 기능의 추가
// - 정적인 기능의 추가 : 코드를 작성할 때, 기능이 추가되는 것
// - 클래스에 기능을 추가하는 것
class LeftMissle : public SpaceShip
{
public:
    void Fire()
    {
        SpaceShip::Fire(); // 원래 기능 사용
        cout << "Fire Left Missile" << endl;
    }
};

// 2) 포함을 사용한 기능의 추가
// - 클래스가 아닌 객체에 기능을 추가할 수 있다
// - 실행 시간에 동적으로 기능이 추가된다.
class RightMissile : public SpaceShip
{
    SpaceShip *ss; // 원본 객체 --> 포함
public:
    RightMissile(SpaceShip *p) : ss(p) {}
    void Fire()
    {
        ss->Fire(); // 원본 객체의 기능을 수행하고
        cout << "Fire Left Missile" << endl;
    }
};

int main()
{
    SpaceShip ss;
    ss.Fire();

    LeftMissle lm;
    lm.Fire();

    //OCP위반!
    //RightMissile rm(&lm); // error.- 공통의 부모가 필요하다.
    RightMissile rm(&ss);
    rm.Fire();

    // Interface를 추가함으로써
    // 공통 부모형태로 넣어주는 형태가 되어서
    // 아래의 식이 성립하는 것을 확인할 수 있다
    I_SpaceShip iss;
    MiddleMissile mdms(&iss);
    mdms.Fire();
    DownMissile dms(&mdms);
    dms.Fire();
};