#include<iostream>

using namespace std;

class Shape
{
public :
	virtual void Draw() = 0;
	virtual Shape* clone() = 0;
};

class Rect : public Shape 
{
public :
	void Draw()
	{
		cout << "draw rectangle" << endl;
	}
	Shape* clone() { return new Rect; }
};

class Triangle : public Shape 
{
public :
	void Draw()
	{
		cout << "draw triangle" << endl;
	}
	Shape* clone() { return new Triangle; }
};

int main()
{
	Shape* rect = new Rect;
	Shape* triangle = new Triangle;
	Shape* p = 0;
	p = rect->clone();
	p->Draw();
	p = triangle->clone();
	p->Draw();


	return 0;
}