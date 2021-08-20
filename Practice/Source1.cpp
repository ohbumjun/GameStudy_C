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
			cout << sz + 1 << ". �����޴���" << endl;
			cout << "�޴��� �����ϼ��� : " ;
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
	PopUpMenu* menubar = new PopUpMenu("�޴���");
	PopUpMenu* p1 = new PopUpMenu("�Ҹ�����");
	PopUpMenu* p2 = new PopUpMenu("ȭ�鼳��");
	menubar->addMenu(p2);
	p1->addMenu(new MenuItem("��������"));
	p1->addMenu(new MenuItem("��������"));
	p2->addMenu(p1);
	p2->addMenu(new MenuItem("��� ����"));
	menubar->command();

	return 0;
}