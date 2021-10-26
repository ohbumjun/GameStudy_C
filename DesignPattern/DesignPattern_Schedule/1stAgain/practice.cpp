#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class ICommand;
class RemoteController // Invoker
{
    // invoker should be instantiated with
    // 4 commands in this case
    Invoker(
        ICommand volumeOn,
        ICommand volumeOff,
        ICommand channelUp,
        ICommand channelDown)
    {
        this.volumeOn = volumeOn;
        this.volumeOff = volumeOff;
        this.channelUp = channelUp;
        this.channelDown = channelDown;
    }
    // since Invoker has Command INstance variables
    // it means, it has the capability of turning the
    // lgith on ,off ... etc
    // using the variables below
    ICommand volumeOn;
    ICommand volumeOff;
    ICommand channelUp;
    ICommand channelDown;

public:
    void clickOn() { this.on.execute(); }
    void clickOff() { this.off.execute(); }
    void clickUp() { this.up.execute(); }
    void clickDown() { this.down.execute(); }
}

class Shape
{
public:
    virtual void Draw() = 0;
    virtual ~Shape() {}
}

struct ICommand
{
    virtual void execute() = 0;
    virtual bool canUnexecute() { return false; }
    virtual void unexecute() {}
    virtual ~ICommand() {}
}

// 도형을 추가하는 명령은 유사한 코드가 많다
// 부모를 만들어서 제공하자
// 동일한 코드는 부모로
// 변하는 코드는 가상함수화 해서 자식이 !!
class AddCommand : public ICommand
{
    int &volume;
    int &channel;

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