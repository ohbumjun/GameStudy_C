
// 패턴중 "Method"가 들어 가는 패턴은 모두 변하는 것을
// 가상함수화 해서
// 자식이 변경하는 패턴입니다. 2개가 있습니다.

// Template method : 전체 알고리즘을 부모가 가지고 각 단계별 변하는 것을
//					가상함수화 해서 자식이 변경(정책, 또는 세부 알고리즘변경)

// Factory Method : 객체 생성을 위한 순수 가상 함수를 만들어 사용하지만
//					어떤객체를 만들지는 자식이 결정...

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Shape
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

struct ICommand
{
    virtual void Execute() = 0;
    virtual bool CanUndo() { return false; }
    virtual void Undo() {}
    virtual ~ICommand() {}
};
// 도형을 추가하는 명령은 유사한 코드가 많다. 부모를 만들어서 제공하자.
// 동일한 코드는 부모로!!
// 변하는 코드는 가상함수화 해서 자식이!!
class AddCommand : public ICommand
{
    vector<Shape *> &v;

public:
    AddCommand(vector<Shape *> &a) : v(a) {}
    virtual void Execute() { v.push_back(CreateShape()); }
    virtual bool CanUndo() { return true; }
    virtual void Undo()
    {
        Shape *p = v.back();
        v.pop_back();
        delete p;
    }

    // Factory Method
    // 객체를 만들기 위한 인터페이스는 정의 하지만 어떤 객체를 만들지는
    // 자식 클래스가 결정하게 하는 디자인 패턴
    // template method 의 모양이지만 가상함수가 하는일이 객체 생성을 결정할때

    virtual Shape *CreateShape() = 0;
};

class AddRectCommand : public AddCommand
{
public:
    AddRectCommand(vector<Shape *> &a) : AddCommand(a) {}

    virtual Shape *CreateShape() { return new Rect; }
};

class AddCircleCommand : public AddCommand
{
public:
    AddCircleCommand(vector<Shape *> &a) : AddCommand(a) {}

    virtual Shape *CreateShape() { return new Circle; }
};

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

int main()
{
    vector<Shape *> v;
    stack<ICommand *> cmd_stack;

    while (1)
    {
        int cmd;
        cin >> cmd;
        if (cmd == 1)
        {
            ICommand *p = new AddRectCommand(v);
            p->Execute();
            cmd_stack.push(p);
        }
        else if (cmd == 2)
        {
            ICommand *p = new AddCircleCommand(v);
            p->Execute();
            cmd_stack.push(p);
        }
        else if (cmd == 9)
        {
            ICommand *p = new DrawCommand(v);
            p->Execute();
            cmd_stack.push(p);
        }
        else if (cmd == 0)
        {
            ICommand *p = cmd_stack.top();
            cmd_stack.pop();

            if (p->CanUndo())
                p->Undo();

            delete p; // delete 하지 말고 다시 p를 스택에 넣으면
                      // redo도 가능합니다.
        }
    }
}