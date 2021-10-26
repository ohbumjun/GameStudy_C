//팩토리 패턴

//사용중이던 객체도 등록하면 유용하다

//프로토 타입 패턴 + 팩토리 패턴

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
    virtual Shape *Clone() = 0; // 자신의 복사본을 만드는 가상함수
    virtual ~Shape() {}
};

class Rect : public Shape
{
public:
    void Draw() { cout << "Rect Draw" << endl; }

    virtual Shape *Clone() { return new Rect(*this); } // 복사생성자
};

class Circle : public Shape
{
public:
    void Draw() { cout << "Circle Draw" << endl; }

    virtual Shape *Clone() { return new Circle(*this); }
};

class ShapeFactory
{
    MAKE_SINGLETON(ShapeFactory)

    map<int, Shape *> prototype;

public:
    void RegisterPrototype(int type, Shape *p)
    {
        prototype[type] = p; // 견본 제품을 등록한다.
    }
    Shape *CreateShape(int type)
    {
        Shape *p = 0;
        if (prototype[type] != 0)
            p = prototype[type]->Clone();

        return p;
    }
};
int main()
{
    vector<Shape *> v;
    ShapeFactory &factory = ShapeFactory::getInstance();

    // 공장에 견본을 등록해 놓는다.
    factory.RegisterPrototype(1, new Rect);
    factory.RegisterPrototype(2, new Circle);
    //factory.RegisterPrototype( 3, new Rect(빨간색, 크기5));
    //factory.RegisterPrototype( 4, new Rect(노란색, 크기20));

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
