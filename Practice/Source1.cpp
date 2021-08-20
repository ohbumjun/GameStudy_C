#include<iostream>
#include<string>
#include<vector>

using namespace std;

class BaseMenu
{
private :
	string m_Name;
public :
	BaseMenu(string n):m_Name(n){}
public :
	string getName() { return m_Name; }
	virtual void command() = 0;
};

class PopUpMenu : public BaseMenu
{
	vector<BaseMenu*>v;
public :
	PopUpMenu(string name):BaseMenu(name){}
public :
	void addMenu(BaseMenu* menu) { v.push_back(menu); }
	virtual void command() override
	{
		int sz = v.size();
		while (true)
		{
			system("cls");
			for (int i = 0; i < sz; i++)
			{
				cout << i+1 << ". " << v[i]->getName() << endl;
			}
			cout << sz + 1 << ". 상위메뉴로" << endl;
			cout << "메뉴를 선택하세요 : " ;
			int _Menu;
			cin >> _Menu;
			if (_Menu <= 0 || _Menu > sz + 1) continue;
			if (_Menu == sz + 1) break;
			v[_Menu - 1]->command();
		}
	}
};

class MenuItem : public BaseMenu
{
public :
	MenuItem(string name) : BaseMenu(name){}
public :
	virtual void command() override
	{
		cout << getName() << endl;
	}
};

int main()
{
	PopUpMenu* menubar = new PopUpMenu("메뉴바");
	PopUpMenu* p1 = new PopUpMenu("소리설정");
	PopUpMenu* p2 = new PopUpMenu("화면설정");
	menubar->addMenu(p2);
	p1->addMenu(new MenuItem("음량설정"));
	p1->addMenu(new MenuItem("음질설정"));
	p2->addMenu(p1);
	p2->addMenu(new MenuItem("밝기 설정"));
	menubar->command();

	return 0;
}