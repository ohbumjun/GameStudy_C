#include<iostream>
#include<vector>
#include<map>

using namespace std;

class Shape;
class RegisterShape;

#define MAKE_SINGLETON(classname) \
private : \
	classname(){}\
	classname(const classname&); \
	void operator = (const classname&); \
public :	 \
	static classname& getInstance()\
	{\
		static classname instance; \
		return instance; \
	}

class Shape
{
public :
	virtual void Draw() = 0;
	virtual ~Shape(){}
};

class ShapeFactory
{
	MAKE_SINGLETON(ShapeFactory);
	typedef Shape* (*CREATOR)();
	map<int, CREATOR>creator_map;
public :
	void RegisterShape(int type, CREATOR creator)
	{
		creator_map[type] = creator;
	}
	Shape* CreateShape(int type)
	{
		Shape* p = 0;
		if (creator_map[type] != 0)
			p = creator_map[type]();
		return p;
	}
};

class Circle : public Shape
{
public :
	void Draw() { cout << "draw circle" << endl; }
	static Shape* CreateObject() { return new Circle; }
};

class Rect : public Shape
{
public :
	void Draw() { cout << "draw Rect" << endl; }
	static Shape* CreateObject() { return new Rect; }
};


int main()
{
	vector<Shape*> v;
	ShapeFactory& factory = ShapeFactory::getInstance();
	factory.RegisterShape(1, &Rect::CreateObject);
	factory.RegisterShape(2, &Circle::CreateObject);

	while (true)
	{
		int cmd;
		cin >> cmd;
		if (cmd >= 1 && cmd <= 5)
		{
			Shape* p = factory.CreateShape(cmd);
			if (p != 0) v.push_back(p);
		}
		if (cmd == 9)
		{
			for (int i = 0; i < v.size(); i++)
				v[i]->Draw();
		}
	}

	return 0;
}