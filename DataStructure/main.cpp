/*
컴포지트 패턴

- 복합객체는 개별객체뿐 아니라, 복합객체 자신도 보관한다.
복합 객체와 개별객체는 동일한 부모가 필요하다
same interface를 따라야 한다.

모든 자식의 공통적 특징은 반드시 부모에도 있어야 한다
그래야 부모 포인터로 자식을 사용할 때 해당하는 특징을 상요할 수 있다.

*/

#include<iostream>
#include<string>
#include<vector>
#include<conio.h>

// 모든 메뉴 공통적 부모
class BaseMenu
{
	std::string title;
public :
	BaseMenu(std::string s) : title(s){}
	// 모든 부모는 소멸자가 가상함수이어야 한다.
	virtual ~BaseMenu(){}

	std::string getTitle() { return title; }
	virtual void command() = 0; 
};

class MenuItem : public BaseMenu
{
	int id;
public :
	MenuItem(std::string s, int n) : BaseMenu(s), id(n){}
	// 메뉴 선택시 호출되는 함수
	void command()
	{
		std::cout << getTitle()  << " 메뉴가 선택됨" << std::endl;
		_getch();
	}
};

class PopMenu : public BaseMenu
{
	std::vector<BaseMenu* > v;

public :
	PopMenu(std::string s) : BaseMenu(s){}
	// Folder도 아래처럼 만들어 달라
	~PopMenu()
	{
		for (int i = 0; i < v.size(); i++) delete v[i];
	}
	void addMenu(BaseMenu* p) { v.push_back(p); }

	// PopMenu를 선택할 때 
	void command()
	{
		while (1)
		{
			system("cls"); // 화면 지우기
			int sz = v.size();
			for (int i = 0; i < sz; i++)
			{
				std::cout << i + 1 << ". " << v[i]->getTitle() << std::endl;
			}
			std::cout << sz + 1 << ". 상위 메뉴로" << std::endl;
			std::cout << "메뉴를 입력하세요 >> ";
			int cmd;
			std::cin >> cmd;

			if (cmd == sz + 1) // 상위 메뉴로 선택
				break;
			if (cmd < 1 || cmd > sz + 2)  // 잘못된 입력
				continue;
			// 선택된 메뉴를 실행합니다 
			v[cmd - 1]->command();
		}
	}
};


int main()
{
	PopMenu* menubar = new PopMenu("메뉴바");
	PopMenu* p1 = new PopMenu("화면 설정");
	PopMenu* p2 = new PopMenu("소리 설정");

	menubar->addMenu(p1);
	p1->addMenu(p2);

	p1->addMenu(new MenuItem("해상도 변경", 11));
	p1->addMenu(new MenuItem("명암   변경", 12));
	p1->addMenu(new MenuItem("채도   변경", 13));

	p2->addMenu(new MenuItem("크기 변경", 21));
	p2->addMenu(new MenuItem("음색 변경", 22));

	// 이제 시작하려면?
	menubar->command();

	delete menubar;
}