// 모든 윈도우의 인터페이스가 있으면 편리하다
//그냥 코드를 한번 보세요

#include <iostream>
#include <string>

using namespace std;

// Implementor
struct IWindowImpl
{
    virtual void Create() = 0;
    virtual void SetColor(string) = 0;
    virtual ~IWindowImpl() {}
};

// Concrete Implementor
class XPWindow : public IWindowImpl
{
public:
    void Create() { cout << "XPWindow Create" << endl; }
    void SetColor(string color) { cout << color << "로 변경" << endl; }
};
class LinuxWindow : public IWindowImpl
{
public:
    void Create() { cout << "LinuxWindow Create" << endl; }
    void SetColor(string color) { cout << color << "로 변경" << endl; }
};

// ----------------------------------
// 구현을 사용하는 인터페이스 클래스 ( Abstraction )
class Window
{
    IWindowImpl *pImpl;

public:
    Window(IWindowImpl *p = 0) : pImpl(p)
    {
        if (pImpl == 0)
            pImpl = new XPWindow;
    }
    ~Window()
    {
        if (pImpl)
            delete pImpl;
    }
    void Create() { pImpl->Create(); }
    void SetColor(string color) { pImpl->SetColor(color); }
};

int main()
{
    // 이제 사용자는 IWindowImpl가 아닌
    // Window만 사용하면 된다.
    // 그리고, 내가 원하는 것만 Window 클래스 내에
    // if (pImpl == 0)
    //        pImpl = new XPWindow;
    // 부분만 바꿔서 껴가면 되는 원리인 것이다.
    Window w;
    w.Create();
    w.SetColor("빨간색");
}