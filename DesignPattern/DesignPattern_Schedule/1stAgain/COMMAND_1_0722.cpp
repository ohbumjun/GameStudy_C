// 커맨트 패턴
// 요청(명령)을 캡슐화 해서 기록했다가,
// 취소 등을 가능하게 하는 패턴
// 요청을 다른 함수에 인자로 전달할 수도 있다

#include <iostream>
#include <vector>

using namespace std;

class Shape // Invoker
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

// 모든 명령을 기록했다가, 나중에 취소할 수 있게 하기 위해
// 모든 명령을 객체화 한다
struct ICommand
{
    virtual void Execute() = 0;
    virtual bool CanUnDo() { return false; }
    virtual void Undo() {}
    virtual ~ICommand() {}
}

// 도형 편집기에서 사용하는 각종 명령을 수행하는
// 객체를 만든다
class AddRectCommand : public ICommand
{
    // &v : 참조 변수라는 것이 중요하다
    // 왜냐하면, command의 경우 , 모두 같은
    // data를 공유할 것이기 때문이다.
    vector<Shape *> &v;

public:
    AddRectCommand(vector<Shape *> &a) : v(a) {}
    // act upon its recevier
    void execute() { v.push_back(new Rect); }
    bool CanUndo() { return true; }
    void Undo()
    {
        Shape *p = v.back();
        v.pop_back();
        delete p;
    }
};

class AddCircleCommand : public ICommand
{
    vector<Shape *> &v;

public:
    AddCircleCommand(vector<Shape *> &a) : v(a) {}
    void Execute() { v.push_back(new Circle); }
    bool CanUndo() { return true; }
    void Undo()
    {
        Shape *p = v.back();
        v.pop_back();
        delete p;
    }
};

class DrawCommand : public ICommand
{
    vector<Shape *> &v;

public:
    DrawCommand(vector<Shape *> &a) : v(a) {}
    void Execute()
    {
        for (int i = 0; i < v.size(); i++)
            v[i]->Draw();
    }
    bool CanUndo() { return true; }
    void Undo() { system("cls"); }
}

#include <stack>

int
main()
{
    // receiver
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

            if (p->CanUnDo())
                p->Undo();
            delete p;
        }
    }