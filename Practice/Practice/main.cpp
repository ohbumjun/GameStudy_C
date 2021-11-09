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

// Abstration : Imeplentor�� ���� Reference ����
// �� �ȿ��� �������� �޼��带 ȣ���Ѵ� 
// ��, �����κп� �޼��� �� ������ �����ϴ� ���̴�
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

// �긴�� ���ϰ� ����� ������ ����
// �Ѵ� interface �ȿ�, detail�� ���߰��� �ϸ�
// �������� ���̴� ����

// ������, ������ ���̰� �ִ� 
// 1) ����� : � Ŭ������ �������̽���
// �ٸ� �ڵ忡�� ����ϴ� �Ͱ� �ٸ� ��
// ��, ����� ���� ��, �Լ����� �ٸ� ��
// ����͸� �߰��� ���� ���ߴ� ��

// �긴�� ������, �߻�� ������ �и��ϴ� ��
// �߻� Ŭ������, �߻� Ŭ���� ���
// ������ �������, ���� ������ ���� �ʴ´�

// ��, ����ʹ� �ᱹ � �ڵ忡 �°Բ�
// ������ �ڵ带 ���� ���� ���ǰ�,
// �긴���� Ȯ�强�� ����ϴ� �ڵ� 