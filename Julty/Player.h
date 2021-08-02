#pragma once

#include "GameInfo.h"

// CMonster 클래스를 전방선언 하여 CMonster 라는 클래스가 어딘가에 있다 라고 가정하고 사용할 수 있게 한다.
// 전방선언시 이 타입을 실제로 생성하는것은 불가능하고 이 타입에 대한 메모리 주소만을 사용할 수 있다.
// 즉, 포인터로만 사용이 가능하다는 것이다.
class CMonster;
class CPlayer
{
public:
    CPlayer();
    ~CPlayer();

private:
    // 아래처럼 실제 객체를 만들기 위해서는 반드시 이 클래스가 있는 헤더파일을 include를 하고
    // 객체를 만들어서 사용해야 한다.
    //CMonster    m_Monster;
    CMonster* m_pMonster;

public:
    void Output();
};

