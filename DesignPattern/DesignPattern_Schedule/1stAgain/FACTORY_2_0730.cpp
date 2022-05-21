//팩토리 패턴
// 공장에 객체를 등록해서 사용할 수 있도록 하자

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

class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
};

class ShapeFactory
{
    MAKE_SINGLETON(ShapeFactory);

    // Shape* 를 리턴하는 함수포인터를
    // CREATOR 라고 정의하는 것이다
    typedef Shape *(*CREATOR)();
    /*
    >> 함수포인터 -------------------
    - 함수의 주소를 가지고 오는 것이다
    - 오로지 함수를 호출할 때만 사용가능하다
    int (*fcnptr)() = func;
    cout << fcnptr() << endl;

    // 중간에 함수포인터 교체가능
    fcnptr = goo;
    cout << fcnptr() << endl;

    // 인자가 존재하는
    // 함수포인터
    int (*fcnptrP)(int) = funcP;
    cout << fcnptrP(1) << endl;
    */
    map<int, CREATOR> creator_map;

public:
    void RegisterShape(int type, CREATOR creator)
    {
        creator_map[type] = creator;
    };
    Shape *CreateShape(int type)
    {
        Shape *p = 0;
        if (creator_map[type] != 0)
            // 등록된 함수를 사용해서
            // 제품을 만들어낸다.
            p = creator_map[type]();
        return p;
    }
};

// CreateObject
// --> 자기자신형을 반환하는 객체가 있으면 편리하다
class Circle : public Shape
{
public:
    void Draw() { cout << "Circle Draw" << endl; }
    static Shape *CreateObject() { return new Circle; }
};

class Rect : public Shape
{
public:
    void Draw() { cout << "Rect Draw" << endl; }
    static Shape *CreateObject() { return new Rect; }
};

int main()
{
    vector<Shape *> v;
    ShapeFactory &factory = ShapeFactory::getInstance();
    // 공장에 제품을 등록한다
    factory.RegisterShape(1, &Rect::CreateObject);
    factory.RegisterShape(2, &Circle::CreateObject);

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
                v[i]->Draw(); //
        }
    }
    return 0;
}
