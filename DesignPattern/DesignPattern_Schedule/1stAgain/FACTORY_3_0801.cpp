//팩토리패턴
//공장에 객체를 등록해서 사용할 수 있도록 하자
//객체 등록과정을 매크로로 제작해보자

/* ---------------------------------------------

- lets' encapsulate that instantiation
( you don't want to know how to make that object
why you made that object etc )
- make it uniform across all places
- Factory is responsible for instantiation.

If there are 2 factories, 
- create both "class I"
but in different way or
create different sub types 

-------------------------------------------- */

#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define MAKE_SINGLETON(classname)      \
private:                               \
    classname() {}                     \
    classname(const classname &);      \
    void operator=(const classname &); \
                                       \
public:                                \
    static classname &getInstance()    \
    {                                  \
        static classname instance;     \
        return instance;               \
    }

//****************************************************************************************************************
class RegisterShape;
class Shape;

// 제품을 자동으로 공장에 등록해 주는 매크로를 도입하자.
#define REGISTER_SHAPE()          \
public:                           \
    static Shape *CreateObject(); \
    static RegisterShape rs;

#define IMPLEMENT_REGISTER(classname, type)                    \
    Shape *classname::CreateObject() { return new classname; } \
    RegisterShape classname::rs(type, &classname::CreateObject);

//****************************************************************************************************************

class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
};

class ShapeFactory
{
    MAKE_SINGLETON(ShapeFactory)

    typedef Shape *(*CREATOR)();

    map<int, CREATOR> creator_map;

public:
    void RegisterShape(int type, CREATOR creator)
    {
        creator_map[type] = creator;
    }

    Shape *CreateShape(int type)
    {
        Shape *p = 0;
        if (creator_map[type] != 0)
            p = creator_map[type](); // 등록된 함수를사용해서 제품을 만든다

        return p;
    }
};

//****************************************************************************************************************

//다음과 같은 클래스를 제작해서 구현한다.
// 살짝 원래 factory 함수의 wrapper 버전인 것 같다
// 즉, 팩토리패턴_3 에서는, 각각의 Shape 자식 클래스들에게
// 자기 자신을 "생산해내는" 함수를 제작했었다
// 여기에서는 추가적으로, 자기 자신을 "등록하는"
// 함수를 제작하려고 한다
class RegisterShape
{
public:
    // 즉, 생성자에 아래와 같이 정의해줬다는 의미는
    // RegisterShape class를 생성함과 동시에
    // 아래의 로직을 실행하겠다는 의미이다.
    RegisterShape(int type, Shape *(*f)())
    {
        //ShapeFactor는 싱글톤으로 관리되야 한다 왜?
        ShapeFactory &factory = ShapeFactory::getInstance();
        factory.RegisterShape(type, f);
    }
};

//****************************************************************************************************************

//CreatObject
//	=> 자기자신형을 반환하는 객체가 있으면 여러모로 편리하다
class Circle : public Shape
{
public:
    void Draw() { cout << "Circle Draw" << endl; }
    REGISTER_SHAPE()
};
IMPLEMENT_REGISTER(Circle, 1);

class Rect : public Shape
{
public:
    void Draw() { cout << "Rect Draw" << endl; }
    REGISTER_SHAPE()
};
IMPLEMENT_REGISTER(Rect, 2);

//****************************************************************************************************************

int main()
{

    vector<Shape *> v;

    ShapeFactory &factory = ShapeFactory::getInstance();

    // 공장에 제품을 등록한다.
    //factory.RegisterShape(1, &Rect::CreateObject);
    //factory.RegisterShape(2, &Circle::CreateObject);

    while (1)
    {
        int cmd;
        cin >> cmd;
        if (cmd > 0 && cmd < 5)
        {
            Shape *p = factory.CreateShape(cmd);
            if (p != 0)
                v.push_back(p);
        }
        else if (cmd == 9)
        {
            for (int i = 0; i < v.size(); i++)
                v[i]->Draw(); // 가상함수(다형성) OCP를 만족한다.
        }
    }
}