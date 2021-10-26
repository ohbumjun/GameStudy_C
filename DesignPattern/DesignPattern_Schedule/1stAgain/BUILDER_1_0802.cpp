/*
디자인 패턴에서 변하는 것을, 클래스로 만들어서
인터페이스 기반으로 교체하는 패턴은 총 3가지 이다.

1) Strategy
: 알고리즘을 캡슐화 하여, 실행시간에 교체가능하게 하는 것
- 오목 게임에서 다음 수를 예측하는 알고리즘을 클래스로

2) State
- 객체에 상태에 따라, 동작을 정의한 클래스를 만드어서
교체게임 캐릭터가 아이템을 획득하면 동작이 달라진다.

3) Builder
- 동일한 구축공정( Director ) 로 객체를 만들지만, 
각 공정에 따른, 표현이 달리지는 객체를 만들 때,
축구 게임에서 모든 캐릭터는 모자/유니폼/신발이 있지만
어떤 모자를 사용하는지는 달라진다.

*/
#include <iostream>
#include <string>

using namespace std;

typedef string Character;

struct IBuilder
{
    Character s;
    virtual void makeHat() = 0;
    virtual void makeUniform() = 0;
    virtual void makeShoes() = 0;
    Character getResult() { return s; }
};

class KoreanBuilder : public IBuilder
{
    virtual void makeHat() override
    {
        s += "삿갓\n";
    }
    virtual void makeUniform() override
    {
        s += "한복\n";
    }
    virtual void makeShoes() override
    {
        s += "짚신\n";
    }
};

class EnglishBuilder : public IBuilder
{
    virtual void makeHat() override
    {
        s += "모자\n";
    }
    virtual void makeUniform() override
    {
        s += "양복\n";
    }
    virtual void makeShoes() override
    {
        s += "구두\n";
    }
};

class Director
{
    IBuilder *pBuilder;

public:
    void setBuilder(IBuilder *p)
    {
        pBuilder = p;
    }
    Character Construct()
    {
        pBuilder->makeHat();
        pBuilder->makeUniform();
        pBuilder->makeShoes();
        return pBuilder->getResult();
    }
};

int main()
{
    KoreanBuilder kb;
    EnglishBuilder eb;
    Director d;
    d.setBuilder(&kb);
    cout << d.Construct() << endl;

    d.setBuilder(&eb);
    cout << d.Construct() << endl;

    return 0;
}