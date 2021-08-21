#include<iostream>
#include<map>
#include<vector>

using namespace std;

class Shape;
class RegisterShape;

#define MAKE_SINGLETON(classname) \
private : \
	classname(){} \
	classname(const classname&);\
	void operator = ( const classname & );\
public : \
	static classname& getInstance() \
	{ \
		static classname instance; \
		return instance; \
	}

#define REGISTER_SHAPE()          \
public:                           \
    static Shape *CreateObject(); \
    static RegisterShape rs;

#define IMPLEMENT_REGISTER(classname, type)                    \
    Shape *classname::CreateObject() { return new classname; } \
    RegisterShape classname::rs(type, &classname::CreateObject);


class Shape
{
public :
    virtual void Draw() = 0;
    virtual ~Shape() {}
};


class ShapeFactory
{
    MAKE_SINGLETON(ShapeFactory);

    typedef Shape* (*CREATOR)();

    map<int, CREATOR> creator_map;

public:
    void RegisterShape(int type, CREATOR creator)
    {
        creator_map[type] = creator;
    }

    Shape* CreateShape(int type)
    {
        Shape* p = 0;
        if (creator_map[type] != 0)
            p = creator_map[type](); // 등록된 함수를사용해서 제품을 만든다

        return p;
    }
};

class RegisterShape
{
public:
    RegisterShape(int type, Shape* (*f)())
    {
        ShapeFactory& factory = ShapeFactory::getInstance();
        factory.RegisterShape(type, f);
    }
};

class Circle : public Shape
{
public :
	void Draw() { cout << "Draw Circle" << endl; }
	REGISTER_SHAPE();
};
IMPLEMENT_REGISTER(Circle,1);

class Rect : public Shape
{
public :
    void Draw() { cout << "Draw Circle" << endl; }
    REGISTER_SHAPE();
};
IMPLEMENT_REGISTER(Rect, 2);



int main()
{
    vector<Shape*>v;
    ShapeFactory& factory = ShapeFactory::getInstance();

    while (true)
    {
        int cmd;
        cin >> cmd;
        if (cmd >= 1 && cmd <= 5)
        {
            Shape* p = factory.CreateShape(cmd);
            if (p != 0)v.push_back(p);

        }
        if (cmd == 9)
        {
            for (int i = 0; i < v.size(); i++)
                v[i]->Draw();
        }
    }
}