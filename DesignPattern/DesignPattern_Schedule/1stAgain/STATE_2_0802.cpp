// state 패턴
// 객체의 상태를 동적으로 바꿔보자

// 방법 2. 변하는 것을 가상함수로

#include <iostream>

using namespace std;

// 바꿔야할 클래스이다
class Character
{
    int gold;
    int item;

public:
    virtual void run() { cout << "run" << endl; }
    virtual void attack() { cout << "attack" << endl; }
}

// 아이템에 따라, 동작을 변경한 다양한 자식 클래스들
class FastCharacter : public Character
{
public:
    virtual void run() { cout << "fast run" << endl; }
    virtual void attack() { cout << "power attack" << endl; }
}

int
main()
{
    Character *p = new Character;
    p->run();
    p->attack();

    p = new FastCharacter; // 아이템 획득..
    p->run();
    p->attack();
}