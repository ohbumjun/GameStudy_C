//팩토리메소드

// 디자인 패턴 23가지중에서 객체를 만들때 사용하는 패턴은 5가지가 있습니다.

//객체 생성부분을 순수가상함수화 해서 자식클래스에 위임하는 기술

//문제
//	 => 버튼과 에디트는 XP,MAC 종류가 있다.
//	 => 사용자 입맛대로 버튼 에디트를 만들 수 있어야 한다.

#include <iostream>

using namespace std;

// 모든 컨트롤은 인터페이스가 필요 하다.
struct IButton
{
    virtual void Draw() = 0;
};
struct IEdit
{
    virtual void Draw() = 0;
};
//-------------------------------
struct XPButton : public IButton
{
    void Draw() { cout << "XPButton Draw" << endl; } //
};
struct XPEdit : public IEdit
{
    void Draw() { cout << "XPEdit Draw" << endl; }
};
//----------------------
struct MACButton : public IButton
{
    void Draw() { cout << "MACButton Draw" << endl; }
};
struct MACEdit : public IEdit
{
    void Draw() { cout << "MACEdit Draw" << endl; }
};
//--------------------------------------------
// 다이얼로그의 공통의 로직을 담은 기본 클래스
class BaseDialog
{
public:
    void Init()
    {
        IButton *pBtn = CreateButton();
        IEdit *pEdit = CreateEdit();
        //pBtn->Move(10,10);
        //pEdit->Move(20,20);
        pBtn->Draw();
        pEdit->Draw();
    }
    // "Factory Method" - 객체를 만들기 위한 순수가상함수
    virtual IButton *CreateButton() = 0;
    virtual IEdit *CreateEdit() = 0;
};
class XPDialog : public BaseDialog
{
public:
    virtual IButton *CreateButton() { return new XPButton; }
    virtual IEdit *CreateEdit() { return new XPEdit; }
};

class MACDialog : public BaseDialog
{
public:
    virtual IButton *CreateButton() { return new MACButton; }
    virtual IEdit *CreateEdit() { return new MACEdit; }
};

int main(int argc, char **argv)
{
    XPDialog dlg;
    dlg.Init();
}
