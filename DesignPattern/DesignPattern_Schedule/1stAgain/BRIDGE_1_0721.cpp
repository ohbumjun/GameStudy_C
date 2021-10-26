//브릿지패턴

//상황
// 모든 윈도우의 인터페이스가 있으면 편리하다

//그냥 코드를 한번 보세요

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
struct IWindow
{
    virtual void Create() = 0;
    virtual void SetColor(string) = 0;
    virtual ~IWindow() {}
};

class XPWindow : public IWindow
{
public:
    void Create() { cout << "XPWindow Create" << endl; }
    void SetColor(string color) { cout << color << "로 변경" << endl; }
};
class LinuxWindow : public IWindow
{
public:
    void Create() { cout << "LinuxWindow Create" << endl; }
    void SetColor(string color) { cout << color << "로 변경" << endl; }
};

// 이제 window 자식으로 button을 만든다고 가정해보자
// 아래처럼 해야한다
class IButton : public IWindow
{
    virtual void Init() = 0;
    virtual ~IButton() {}
}

// 각 스타일의 버튼을 다시 만들어야 한다
// 스타일이 많다면, 아주 복잡해진다
// 그렇다면, 어떻게 해야할까 ?
class XPButton : public IButton
{
};
class LinuxButton : public IButton
{
};

int main()
{
}