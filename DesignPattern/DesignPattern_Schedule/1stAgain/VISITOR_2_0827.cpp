/*
각각의 자동차 파트( Wheel, Body, Engine)가 존재하고
컴포지트 패턴을 활용한 Car class 구현

Car class 에서는
각각의 Part Element를 추가하여
accept method를 호출하여
visitor 패턴을 호출

visitor 클래스는 각각 자동차를 구동하는
기능 클래스인
CarStartVisitor와

자동차 상태를 점검하는 클래스인
CarPrintVisitor가
구현되어 있다

*/

/*
각각의 자동차 파트( Wheel, Body, Engine)가 존재하고
컴포지트 패턴을 활용한 Car class 구현

Car class 에서는
각각의 Part Element를 추가하여
accept method를 호출하여
visitor 패턴을 호출

visitor 클래스는 각각 자동차를 구동하는
기능 클래스인
CarStartVisitor와

자동차 상태를 점검하는 클래스인
CarPrintVisitor가
구현되어 있다

*/

#include <list>
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

#if _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif

// Element interface (visitable )
class Visitor;
class Element
{
public:
    virtual void accept(Visitor *v) = 0;
};

// Wheel
class Wheel : public Element
{
private:
    const char *m_Name;

public:
    Wheel(const char *name) : m_Name(name) {}
    // Wheel(const char* name) : m_Name(name) {}

public:
    const char *getName() const
    {
        return m_Name;
    }
    void accept(Visitor *) override;
};

// Engine
class Engine : public Element
{
public:
    void accept(Visitor *) override;
};

// Body
class Body : public Element
{
public:
    void accept(Visitor *v) override;
};

// Car : composite pattern
class Car : public Element
{
private:
    list<Element *> mList;

public:
    void append(Element *e) { mList.push_back(e); }
    void accept(Visitor *v) override;
};

// Visitor
class Visitor
{
public:
    virtual void visit(Wheel *e) = 0;
    virtual void visit(Engine *e) = 0;
    virtual void visit(Body *e) = 0;
    virtual void visit(Car *e) = 0;
};

class CarPrintVisitor : public Visitor
{
public:
    void visit(Wheel *e) override { cout << "Visiting " << e->getName() << endl; }
    void visit(Engine *e) override { cout << "Visiting engine" << endl; }
    void visit(Body *e) override { cout << "Visiting body" << endl; }
    void visit(Car *e) override { cout << "Visiting car" << endl; }
};

class CarStartVisitor : public Visitor
{
public:
    void visit(Wheel *e) override { cout << "Kicking " << e->getName() << endl; }
    void visit(Engine *e) override { cout << "Starting engine" << endl; }
    void visit(Body *e) override { cout << "Moving body" << endl; }
    void visit(Car *e) override { cout << "Starting car" << endl; }
};

void Wheel::accept(Visitor *v) { v->visit(this); }
void Engine::accept(Visitor *v) { v->visit(this); }
void Body::accept(Visitor *v) { v->visit(this); }
void Car::accept(Visitor *v)
{
    for (auto i = mList.begin(); i != mList.end(); i++)
    {
        (*i)->accept(v);
    }
    v->visit(this);
}

int main()
{
    // 부품 생성
    Wheel mWheel[4] = {
        "front left",
        "front right",
        "back left",
        "back right"};
    Engine mEngine;
    Body mBody;
    Car mCar;

    // 파트 추가
    mCar.append(&mWheel[0]);
    mCar.append(&mWheel[1]);
    mCar.append(&mWheel[2]);
    mCar.append(&mWheel[3]);

    mCar.append(&mEngine);
    mCar.append(&mBody);

    // Visitor 생성
    CarPrintVisitor mCarPrintVisitor;
    CarStartVisitor mCarStartVisitor;

    // visitor accept 처리
    mCar.accept(&mCarPrintVisitor);
    mCar.accept(&mCarStartVisitor);

    return 0;
}
