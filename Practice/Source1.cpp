#include<iostream>

using namespace std;

// Implementor
class Drawing
{
public :
	virtual void drawLine(int x, int y) = 0;
	virtual void fill() = 0;
};

class RectDrawing : public Drawing
{
public :
	virtual void drawLine(int x, int y) override
	{
		cout << "draw Rect Line from " << x << " to " << y << endl;
	}
	virtual void fill()
	{
		cout << "fill Rect" << endl;
	}
};

class CircleDrawing : public Drawing
{
public :
	virtual void drawLine(int x, int y) override
	{
		cout << "draw Circle Line from " << x << " to " << y << endl;
	}
	virtual void fill()
	{
		cout << "fill Circle" << endl;
	}
};

// Abstraction
class Shape
{
	Drawing* m_Dr;
public :
	Shape(Drawing*dr):m_Dr(dr){}
	virtual void draw() = 0;
	void drawLine(int x, int y)
	{
		m_Dr->drawLine(x,y);
	}
	void fill()
	{
		m_Dr->fill();
	}
};

class Circle : public Shape
{
public :
	Circle(Drawing*dr):Shape(dr){}
	void draw() { cout << "draw Circle" << endl; }
};

class Rectangle : public Shape
{
public:
	Rectangle(Drawing* dr) :Shape(dr) {}
	void draw() { cout << "draw Rectangle" << endl; }
};

int main()
{
	Shape* circle = new Circle(new CircleDrawing());
	circle->draw();
	circle->drawLine(1, 2);
	circle->fill();
	
	Shape* rectangle = new Rectangle(new RectDrawing());
	rectangle->draw();
	rectangle->drawLine(3,4);
	rectangle->fill();
	
	
	return 0;
}