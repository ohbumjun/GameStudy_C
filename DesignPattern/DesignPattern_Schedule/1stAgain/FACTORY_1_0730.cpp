// 팩토리 패턴

// 객체를 생성하는 공장역할을 하는 객체를 만들어봅시다
// 객체를 생성하는 공장은, 당연히 하나만 만들어야 하므로
// 싱글톤을 활용하자

#include <iostream>
#include <string>
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

class Shape //
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
};

class Rect : public Shape
{
public:
    void Draw() { cout << "Rect Draw" << endl; }
};

class Circle : public Shape
{
public:
    void Draw() { cout << "Circle Draw" << endl; }
};

class Tri : public Shape
{
public:
    void Draw() { cout << "Triangle Draw" << endl; }
};

class ShapeFactory
{
    MAKE_SINGLETON(ShapeFactory);

public:
    Shape *CreateShape(int type)
    {
        Shape *p = 0;

        // 새로운 객체를 찍어내고 싶으면, 다음 코드만 수정하면 된다.
        switch (type)
        {
        case 1:
            p = new Rect;
            break;
        case 2:
            p = new Circle;
            break;
        case 3:
            p = new Tri;
            break;
        }
        return p;
    }
};

int main()
{
    vector<Shape *> v;

    ShapeFactory &factory = ShapeFactory::getInstance();

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
                v[i]->Draw();
        }
    }
}
