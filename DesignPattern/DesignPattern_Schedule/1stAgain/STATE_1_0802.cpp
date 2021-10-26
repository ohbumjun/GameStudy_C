// state 패턴
// 객체의 상태를 동적으로 바꿔보자

// 방법 1. 아이템의 종류에 따른 조건분기
// 문제점 : 신규 아이템이 추가되면 조건분기 문이 추가 되어야 한다.
//			OCP를 만족할수 없다

#include <iostream>

using namespace std;

// 바꿔야할 클래스이다
class Character
{
    int gold;
    int item;

public:
    void run()
    {
        if (item == 1)
            cout << "fast run" << endl;
        else if (item == 2)
            cout << "very fast run" << endl;
        else
            cout << "run" << endl;
    }
    void attack() { cout << "attack" << endl; }
}

int
main()
{
    Character *p = new Character;
    p->run();
    p->attack();
}