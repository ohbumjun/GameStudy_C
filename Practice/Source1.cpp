// 팩토리 메서드 패턴
// 객체 생성을 위한 인터페이스 정의
// 객체 생성 부분을 가상함수화 하여
// 자식 클래스에게 실제 객체 생성 위임
#include<iostream>

using namespace std;

struct IButton
{
	virtual void Draw() = 0;
};
struct IEdit
{
	virtual void Draw() = 0;
};

struct MACButton : public IButton
{
	void Draw() { cout << "draw macBtn" << endl; }
};
struct MACEdit : public IEdit
{
	void Draw() { cout << "draw macEdit" << endl; }
};
struct XPButton : public IButton
{
	void Draw() { cout << "draw xpBtn" << endl; }
};
struct XPEdit : public IEdit
{
	void Draw() { cout << "draw xpEdit" << endl; }
};

class BaseDialog
{
public :
	void Init()
	{
		IButton* pBtn = CreateButton();
		IEdit* pEdit = CreateEdit();
		pBtn->Draw();
		pEdit->Draw();
	}
	virtual IButton* CreateButton() = 0;
	virtual IEdit* CreateEdit() = 0;
};

class XPDialog : public BaseDialog
{
	IButton* CreateButton() { return new XPButton; }
	IEdit* CreateEdit() { return new XPEdit; }
};

class MACDialog : public BaseDialog
{
	IButton* CreateButton() { return new MACButton; }
	IEdit* CreateEdit() { return new MACEdit; }
};

int main()
{
	XPDialog dl;
	dl.Init();
}