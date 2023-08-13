//컴포지트 패턴

// 복합객체는 개별객체뿐 아니라 복합객체 자신도 보관한다.(재귀적 포함)
// => 복합객체와 개별객체는 동일한 부모가 필요하다.

// 복합객체와 개별객체의 사용법이 동일시 된다.(메뉴 선택시 command()호출)
// => 동일시 하는 함수는 부모로 부터 와야 한다.

// 모든 자식의 공통된 특징은 반드시 부모에도 있어야 한다.
// 그래야 부모 포인터로 자식을 사용할때 해당하는 특징을 사용할수 있다.
// LSP : Liskov Substitution Principle

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
using namespace std;

// 모든 메뉴의 공통의 부모
class BaseMenu
{
    string title;

public:
    BaseMenu(string s) : title(s) {}
    virtual ~BaseMenu() {} // 모든 부모는 소멸자가 가상함수이어야한다.

    string getTitle() { return title; }
    virtual void command() = 0; // 모든 자식 메뉴는 꼭 이함수를 만들어라!
};

//---------------------------------
class MenuItem : public BaseMenu
{
    int id;

public:
    MenuItem(string s, int n) : BaseMenu(s), id(n) {}
    // 메뉴를 선택할때 호출되는 함수
    void command()
    {
        cout << getTitle() << "메뉴가 선택됨 " << endl;
        getch();
    }
};
//--------------------------------------------------
// 아래 클래스가 이 디자인의 핵심입니다.
class PopupMenu : public BaseMenu
{
    vector<BaseMenu *> v; // 핵심!!
public:
    PopupMenu(string s) : BaseMenu(s) {}

    // Folder에도 아래처럼 만들어 두세요..!!
    ~PopupMenu()
    {
        for (int i = 0; i < v.size(); i++)
            delete v[i];
    }

    void addMenu(BaseMenu *p) { v.push_back(p); }

    // PopupMenu를 선택할때
    void command()
    {
        while (1)
        {
            system("cls"); // 화면 지우기
            int sz = v.size();
            for (int i = 0; i < sz; i++)
            {
                cout << i + 1 << ". " << v[i]->getTitle() << endl;
            }
            cout << sz + 1 << ". 상위 메뉴로" << endl;
            cout << "메뉴를 입력하세요 >> ";
            int cmd;
            cin >> cmd;

            if (cmd == sz + 1) // 상위 메뉴로 선택!
                break;         // 또는 return

            if (cmd < 1 || cmd > sz + 1) // 잘못된 입력
                continue;

            // 선택된 메뉴를 실행합니다.
            v[cmd - 1]->command();
        }
    }
};

int main()
{
    PopupMenu *menubar = new PopupMenu("메뉴바");
    PopupMenu *p1 = new PopupMenu("화면 설정");
    PopupMenu *p2 = new PopupMenu("소리 설정");

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
