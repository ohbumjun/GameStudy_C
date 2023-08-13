// 프로토 타입 패턴이란 ?
// --> 미리 만들어진 instance를
// clone함수로 복사해서 객체를 생성하는 패턴

// 문제
// 파워포인트에서 도형을 제작하는 하나의 틀을 만들어보자
// draw 함수가 있고, 무슨 도형인지 출력할 수 있다
// 복사를 하면, 이전의 모양 그대로 복사가 되야 한다

// 추가된 점
// Rectangle이 Shape 인터페이스를 구현하게 하자
// Triangle과 Ameba 클래스도 만들어서 실험

#include <iostream>

using namespace std;

class Shape
{
public:
    virtual void Draw() = 0;
    virtual Shape *Clone() = 0;
};

class Rectangle : public Shape
{
public:
    void Draw()
    {
        cout << "rectangle : " << endl;
    }

    virtual Shape *Clone() { return new Rectangle(*this); }
};

class Triangle : public Shape
{
public:
    void Draw()
    {
        cout << "Triangle : " << endl;
    }

    virtual Shape *Clone() { return new Triangle(*this); }
};

class Ameba : public Shape
{
public:
    void Draw()
    {
        cout << "Ameba : " << endl;
    }

    virtual Shape *Clone() { return new Ameba(*this); }
};

int main()
{
    Shape *rect = new Rectangle;
    Shape *triangle = new Triangle;
    Shape *ameba = new Ameba;

    Shape *shape;

    shape = rect->Clone();
    shape->Draw();

    shape = triangle->Clone();
    shape->Draw();

    shape = ameba->Clone();
    shape->Draw();

    return 0;
}