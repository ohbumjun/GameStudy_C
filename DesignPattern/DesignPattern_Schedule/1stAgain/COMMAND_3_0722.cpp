#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
}

class Rect : public Shape
{
public:
    void Draw() { cout << "Rect Draw" << endl; }
}

class Circle : public Shape
{
public:
    void Draw() { cout << "Circle Draw" << endl; }
}

struct ICommand
{
    virtual void Execute() = 0;
    virtual bool CanUndo() { return False; }
    virtual void Undo() {}
    virtual ~ICommand() {}
}

// 도형을 추가하는 명령은 유사한 코드가 많다
// 부모를 만들어서 제공하자
// 동일한 코드는 부모로
// 변하는 코드는 가상함수화 해서 자식이 !!
class AddCommand : public ICommand
{
    vector<Shape *> &v;

public:
    AddCommand(vector<Shape *> &a) : v(a) {}
    virtual void Execute() { v.push_back(CreateShape()); }
    virtual void CanUndo() { return false; }
    virtual void Undo()
    {
        Shape *p = v.back();
        v.pop_back();
        delete p;
    }
    // 팩토리 method
    // 객체를 만들기 위한 인터페이스는 정의하지만
    // 어떤 객체를 만들지는 자식 클래스가 결정하게 하는
    // 디자인 패턴
    // template method의 모양이지만,
    // 가상함수가 하는일이 객체 생성을 결정한다
    virtual Shape *CreateShape() = 0;
}

class AddRectCommand : public AddCommand
{
public:
    AddRectCommand(vector<Shape *> &a) : AddCommand(a) {}
    virtual Shape *CreateShape() { return new Rect; }
}

class AddCircleCommand : public AddCommand
{
public:
    AddCircleCommand(vector<Shape *> &a) : AddCommand(a) {}
    virtual Shape *CreateShape() { return new Circle; }
}

class DrawCommand : public ICommand
{
    vector<Shape *> &v;

public:
    DrawCommand(vector<Shape *> &a) : v(a) {}

    virtual void Execute()
    {
        for (int i = 0; i < v.size(); i++)
            v[i]->Draw();
    }
    virtual bool CanUndo() { return true; }
    virtual void Undo()
    {
        system("cls"); // 화면 지우기
    }
};

// 여러개의 명령을 묶어서 사용하는 매크로 명령
class MacroCommand : public ICommand
{
    // Composite 패턴
    vector<ICommand *> v;

public:
    void AddCommand(ICommand *p) { v.push_back(p); }
    void Execute()
    {
        for (int i = 0; i < v.size(); i++)
            v[i]->Execute();
    }
}

int
main()
{
    vector<Shape *> v;
    stack<ICommand *> cmd_stack;

    MacroCommand *p1 = new MacroCommand;
    p1->AddCommand(new AddRectCommand(v));
    p1->AddCommand(new AddCircleCommand(v));
    p1->AddCommand(new DrawCommand(v));
    p1->Execute();

    MacroCommand *p2 = new MacroCommand;
    p2->AddCommand(new AddRectCommand(v));
    p2->AddCommand(p1);
    p2->AddCommand(new DrawCommand(v));
    p2->Execute();

    return 0;
}