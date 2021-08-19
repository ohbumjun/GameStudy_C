#include<iostream>
#include<vector>
#include<string>

using namespace std;

class BaseMenu
{
private :
	string m_Name;
public :
	BaseMenu(string n) : m_Name(n){}
	string getTitle() { return m_Name; }
	virtual void command() = 0;
};

class MenuItem : public BaseMenu
{
	int m_Id;
public :
	MenuItem(string n, int id) : BaseMenu(n),m_Id(id) {}
	void command()
	{
		cout << getTitle() << " 실행" << endl;
	}
};

class PopUpMenu : public BaseMenu
{
	vector<BaseMenu*> m_VMenu;
public :
	PopUpMenu(string n) : BaseMenu(n){}
	void addMenu(BaseMenu* menu) { m_VMenu.push_back(menu); }
	void command()
	{
		int _Menu;
		int sz = m_VMenu.size();
		while (true)
		{
			system("cls");
			for (int i = 0; i < sz; i++)
				cout << i + 1 << ". " << m_VMenu[i]->getTitle() << endl;
			cout << sz + 1 << ". 상위메뉴로 " << endl;
			cout << "메뉴를 선택하세요 :";
			cin >> _Menu;
			if (_Menu < 1 || _Menu > sz+1) continue;
			if (_Menu == sz+1) break;
			m_VMenu[_Menu-1]->command();
		}
	}
};

int main()
{
	PopUpMenu* menubar = new PopUpMenu("메뉴바");
	PopUpMenu* p1 = new PopUpMenu("화면 설정");
	PopUpMenu* p2 = new PopUpMenu("소리 설정");

	menubar->addMenu(p1);
	p1->addMenu(p2);
	p1->addMenu(new MenuItem("화질 설정", 11));

	p2->addMenu(new MenuItem("음량 설정",21));
	p2->addMenu(new MenuItem("음질 설정", 22));

	menubar->command();

	return 0;
}