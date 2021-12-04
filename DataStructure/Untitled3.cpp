#include <iostream>

using namespace std;

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
    RightMissile rm(&lm); // error.- 공통의 부모가 필요하다.
    // RightMissile rm(&ss);
    rm.Fire();
    
    return 0;
}
