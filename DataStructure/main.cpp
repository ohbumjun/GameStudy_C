/*
������Ʈ ����

- ���հ�ü�� ������ü�� �ƴ϶�, ���հ�ü �ڽŵ� �����Ѵ�.
���� ��ü�� ������ü�� ������ �θ� �ʿ��ϴ�
same interface�� ����� �Ѵ�.

��� �ڽ��� ������ Ư¡�� �ݵ�� �θ𿡵� �־�� �Ѵ�
�׷��� �θ� �����ͷ� �ڽ��� ����� �� �ش��ϴ� Ư¡�� ����� �� �ִ�.

*/

#include<iostream>
#include<string>
#include<vector>
#include<conio.h>

// ��� �޴� ������ �θ�
class BaseMenu
{
	std::string title;
public :
	BaseMenu(std::string s) : title(s){}
	// ��� �θ�� �Ҹ��ڰ� �����Լ��̾�� �Ѵ�.
	virtual ~BaseMenu(){}

	std::string getTitle() { return title; }
	virtual void command() = 0; 
};

class MenuItem : public BaseMenu
{
	int id;
public :
	MenuItem(std::string s, int n) : BaseMenu(s), id(n){}
	// �޴� ���ý� ȣ��Ǵ� �Լ�
	void command()
	{
		std::cout << getTitle()  << " �޴��� ���õ�" << std::endl;
		_getch();
	}
};

class PopMenu : public BaseMenu
{
	std::vector<BaseMenu* > v;

public :
	PopMenu(std::string s) : BaseMenu(s){}
	// Folder�� �Ʒ�ó�� ����� �޶�
	~PopMenu()
	{
		for (int i = 0; i < v.size(); i++) delete v[i];
	}
	void addMenu(BaseMenu* p) { v.push_back(p); }

	// PopMenu�� ������ �� 
	void command()
	{
		while (1)
		{
			system("cls"); // ȭ�� �����
			int sz = v.size();
			for (int i = 0; i < sz; i++)
			{
				std::cout << i + 1 << ". " << v[i]->getTitle() << std::endl;
			}
			std::cout << sz + 1 << ". ���� �޴���" << std::endl;
			std::cout << "�޴��� �Է��ϼ��� >> ";
			int cmd;
			std::cin >> cmd;

			if (cmd == sz + 1) // ���� �޴��� ����
				break;
			if (cmd < 1 || cmd > sz + 2)  // �߸��� �Է�
				continue;
			// ���õ� �޴��� �����մϴ� 
			v[cmd - 1]->command();
		}
	}
};


int main()
{
	PopMenu* menubar = new PopMenu("�޴���");
	PopMenu* p1 = new PopMenu("ȭ�� ����");
	PopMenu* p2 = new PopMenu("�Ҹ� ����");

	menubar->addMenu(p1);
	p1->addMenu(p2);

	p1->addMenu(new MenuItem("�ػ� ����", 11));
	p1->addMenu(new MenuItem("���   ����", 12));
	p1->addMenu(new MenuItem("ä��   ����", 13));

	p2->addMenu(new MenuItem("ũ�� ����", 21));
	p2->addMenu(new MenuItem("���� ����", 22));

	// ���� �����Ϸ���?
	menubar->command();

	delete menubar;
}