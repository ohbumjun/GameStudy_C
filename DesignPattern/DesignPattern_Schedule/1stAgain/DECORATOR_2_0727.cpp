// 데코레이터 패턴
// 객체에 기능을 추가하는 패턴

// 상속의 장점 : 추가 멤버변수가 필요 없다
// 포함의 장점 : 다형성을 적용할 수 있다
// 두가지 장점을 모두 살려보자

#include <iostream>

using namespace std;

struct IComponent
{
    virtual void Fire() = 0;
    virtual ~IComponent(){};
}

class SpaceShip : public IComponent
{
public:
    int color;
    void Fire() { cout << "Fire Missile" << endl; }
}

class LeftMissile : public IComponent
{
    IComponent *ss;

public:
    LeftMissile(IComponent *p) : ss(p) {}
    void Fire()
    {
        ss->Fire();
        cout << "Fire Left Missile" << endl;
    }
} class RightMissile : public IComponent
{
    IComponent *ss;

public:
    RightMissile(IComponent *p) : ss(p) {}
    void Fire()
    {
        ss->Fire();
        cout << "Fire Right Missile" << endl;
    }
}

int
main()
{
    SpaceShip ss;
    ss.Fire();

    LeftMissile lm(&ss);
    lm.Fire();

    RightMissile rm(&lm);
    rm.Fire();
}