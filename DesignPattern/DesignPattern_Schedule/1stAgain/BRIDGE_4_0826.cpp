/*
Bridge Pattern
- decouple an abstraction
from its
implemenation

implementor, abstratcion
각각의 interface 존재

abstraction은
implementor 로 구성

*/

#include <iostream>
#include <string>

using namespace std;

//Implementor : 구현부분을 위한 인터페이스 :
class Drawing
{
public:
    virtual void drawLine(int x, int y) = 0;
    virtual void fill() = 0;
};

// Abstraction : 기능 계층의 최상위 클래스
// - 추상 인터페이스를 정의
// - Implementor에 대한 레퍼런스 유지
// - 구현부분(Implementor)에 해당하는 클래스의 인스턴스 지니고
// 해당 인스턴스를 통해 구현부분의 메서드를 호출한다

// 즉 ! 구현부분을 모두 인터페이스로 위임(Delegation) 함으로써
// 브릿지 패턴의 특성을 지니게 된다.
class Shape
{
private:
    Drawing *drawing;

public:
    Shape(Drawing *dr) : drawing(dr) {}

public:
    virtual void draw() = 0;
    virtual void drawLine(int x, int y)
    {
        drawing->drawLine(x, y);
    }
    virtual void fill()
    {
        drawing->fill();
    }
};

// RefinedAbstraction : Abstraction에 정의된
// 인터페이스를 확장
class Rectangle : public Shape
{
public:
    Rectangle(Drawing *dr) : Shape(dr) {}
    virtual void draw() override
    {
        cout << "draw rect" << endl;
    }
};

class Circle : public Shape
{
public:
    Circle(Drawing *dr) : Shape(dr) {}
    virtual void draw() override
    {
        cout << "draw circle" << endl;
    }
};

// ConcreteImplementor : Implementor 인터페이스를 구현( 실제 기능 구현 )
class RectDrawing : public Drawing
{
public:
    virtual void drawLine(int x, int y) override
    {
        cout << "Rect draw line from " << x << " from " << y << endl;
    }
    void fill() override
    {
        cout << "Rect Fill" << endl;
    }
};

class CircleDrawing : public Drawing
{
public:
    virtual void drawLine(int x, int y) override
    {
        cout << "Circle draw line from " << x << " from " << y << endl;
    }
    void fill() override
    {
        cout << "Circle Fill" << endl;
    }
};

int main()
{
    Shape *rectangle = new Rectangle(new RectDrawing());
    Shape *circleangle = new Circle(new CircleDrawing());

    rectangle->drawLine(1, 2);
    rectangle->fill();

    circleangle->drawLine(3, 4);
    circleangle->fill();
};

// 브릿지 패턴과 어댑터 패턴의 차이
// 두 패턴 모두 interface의 detail을 감추고자 하며
// 구조적인 차이가 없다.

// 하지만, 목적의 차이가 잇다
// 1) 어댑터 : 어떤 클래스의 인터페이스가
// 다른 코드에서 기대하는 것과 다를 때
// 즉, 기능은 같은 데, 함수명이 다를 때
// 어댑터를 중간에 끼워 맞추는 것

// 브릿지 패턴은, 추상과 구현을 분리하는 것이다
// 추상 클래스는 추상 클래스대로
// 구현은 구현대로, 서로 영향을 주지 않는다

// 즉, 어댑터는 결국 어떤 코드에 맞게끔
// 기존의 코드를 쓰기 위해 사용되고,
// 브릿지는 확장성을 고려하는 코드