#include <iostream>
#include <string>

using namespace std;

// Implementor
class Drawing
{
public :
	virtual void drawLine() = 0;
	virtual void fill() = 0;
};

// Abstration : Imeplentor에 대한 Reference 유지
// 이 안에서 구현부의 메서드를 호출한다 
// 즉, 구현부분에 메서드 및 역할을 위임하는 것이다
class Shape
{
private :
	Drawing* drawing;
public :
	Shape(Drawing* dr) : drawing(dr){}
public :
	virtual void draw() = 0;
	virtual void drawLine()
	{
		drawing->drawLine();
	}
	virtual void fill()
	{
		drawing->fill();
	}
};

class Rectangle : public Shape
{
public :
	Rectangle(Drawing* dr) : Shape(dr){}
	virtual void draw() override
	{
		cout << "draw rect" << endl;
	}
};

class Circle : public Shape
{
public:
	Circle(Drawing* dr) : Shape(dr) {}
	virtual void draw() override
	{
		cout << "draw circle" << endl;
	}
};

class RectDrawing : public Drawing
{
public :
	virtual void drawLine() override
	{
		cout << "Rect Draw" << endl;
	}
	virtual void fill() override
	{
		cout << "Rect Fill" << endl;
	}
};

class CircleDrawing : public Drawing
{
public:
	virtual void drawLine() override
	{
		cout << "Circle Draw" << endl;
	}
	virtual void fill() override
	{
		cout << "Circle Fill" << endl;
	}
};

int main()
{
	Shape* rectangle = new Rectangle(new RectDrawing());
	Shape* circle = new Circle(new CircleDrawing());

	rectangle->drawLine();
	rectangle->fill();

	circle->drawLine();
	circle->fill();

	return 0;
}

// 브릿지 패턴과 어댑터 패턴의 차이
// 둘다 interface 안에, detail을 감추고자 하며
// 구조적인 차이는 없다

// 하지만, 목적의 차이가 있다 
// 1) 어댑터 : 어떤 클래스의 인터페이스가
// 다른 코드에서 기대하는 것과 다를 때
// 즉, 기능은 같은 데, 함수명이 다를 때
// 어댑터를 중간에 끼워 맞추는 것

// 브릿지 패턴은, 추상과 구현을 분리하는 것
// 추상 클래스는, 추상 클래스 대로
// 구현은 구현대로, 서로 영향을 주지 않는다

// 즉, 어댑터는 결국 어떤 코드에 맞게끔
// 기존의 코드를 쓰기 위해 사용되고,
// 브릿지는 확장성을 고려하는 코드 