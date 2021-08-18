#include "..\TextRPG_6\Player.h"
#include "..\TextRPG_6\Player.h"
#include "..\TextRPG_6\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include "..\TextRPG\Player.h"
#include<iostream>

using namespace std;
/*
절차지향 프로그래밍
- 순차적으로 처리해주는 프로그래밍 기법
- 장점 : 컴퓨터가 작업 처리하는 것과 매우 유사

객체지향 프로그래밍(OOP)
- 객체(모든사물)을 제작하고, 객체들을 조합하여
하나의 완성된 프로그램을 만들어가는 방식
C++ 에서는 객체 지향 프로그래밍을 지원하기 위한
수단으로 클래스를 제공한다

1) 캡슐화
- 구조체와 같이, 여러 데이터를 혹은
함수를 묶어서 사용할 수 있게 만들어주는 속성
(묶어주는 기능)

2) 은닉화 :
- 구조체나, 클래스의 변수 및 멤버를
외부에 공개하는 수준을 설정할 수 있다
-- 2_1) public :
클래스 혹은 구조체의 내부
(클래스나 구조체에 만들어져 있는 함수 안)와,
외부(클래스의 함수 안이 아니라,
다른 클래스 혹은 다른 함수 안)에서 모두 접근이 가능하다

- 구조체는 아무것도 안붙여줄 경우,
기본으로 public
- 클래스는 아무것도 안붙여줄 경우,
기본으로 private
-- 2_2) protected
-- 2_3) prviate
- 클래스 혹은 구조체의 내부에서만
접근 가능. 외부에서는 접근 불가능

3) 상속성
4) 다형성
*/

struct Test
{
	int x;
	int y;
};

class CPlayer
{
public:
	int m_a;
	int m_Number1;
	int m_Number2;
	// 동적 배열
	int* m_Array;
private:
	int m_Number3; // private
public:
	// 생성자,소멸자
	// 1) 생성자
	// - 이 클래스를 이용하여, 객체를 만들 때, 자동으로 호출되는 함수
	// ( 클래스는, 객체를 만들기 위한 수단이지, 객체가 아니다 )
	// 2) 소멸자 : 객체가 소멸될 때, 자동으로 호출되는 함수이다
	// - 생성자에서는 Initializerf를 사용하여, 초기화할 수 있다
	// ex) int Num2 = 50; 선언과 동시에 초기화
	CPlayer() : m_Number2(300)
	{
		cout << "CPlayer 생성자" << endl;
		// 위에서는 Initializer, 즉, 선언과 동시에 m_Number2를 초기화
		// 하지만, 아래 코드에서는, 선언 후, 별도 초기화 작업을 거치는 것
		// 정리하자면, initializer는 선언과 동시에 초기화 
		m_Number1 = 100;
	}

	// 생성자와 오버로딩
	// 생성자만 overloading이 가능하고
	// 소멸자는 overloading이 불가능하다
	CPlayer(int Num) : m_Number1(Num) {}
	~CPlayer() {
		delete[] m_Array;
		cout << "CPlayer 소멸자" << endl;
	}

	// 여러개 inializer도 가능
	CPlayer(int Num1, int Num2, int Num3) :
		m_Number1(Num1),
		m_Number2(Num2),
		m_Number3(Num3) {}

	// 복사 생성자 : 인자에 const 붙이기
	// 무언가를 복사하기 위한 생성자
	// ex) A객체를 B객체에 넘겨줘서
	// 완전히 동일한 객체를 설명해주는 것이다
	CPlayer(const CPlayer& player)
	{
		cout << "copy constructor" << endl;
		// 1) 얕은 복사 ?
		// 단순히 변수의 값들만을 복사하는 방식
		m_Number1 = player.m_Number1;
		m_Number2 = player.m_Number2;

		// 발생가능한 문제점 ? "댕글링 포인터"
		// 두 객체가 모두 같은 배열의 메모리 주소를 작데 되므로
		// 소멸자에서 해당 동적 배열을 메모리에서 제거할 때
		// 문제가 발생할 수 있다.
		// 즉, A를 B로 복사하는데
		// A에서 동적 배열을 메모리에서 해제하면
		// B는 더이상 참조할 주소에, 기존 동적배열이 없게 된다.
		// 댕글리 포인터가 되버리는 것이다
		m_Array = player.m_Array;

		// 따라서 "깊은 복사"를 해준다.
		// B를 생성시, 값은 같되, 다른 메모리를 할당하고
		// 다른 주소값을 가져오는 것이다.
		// 즉, A와 다른 주소값을 가지게 하는 것이다
		// 메모리 공간도 새로 만들고, 그 공간 안에다가
		// 데이터들만 복사해서 넣는 개념인 것이다
		// --> 다른 말로 하면, m_Array라는 포인터 변수가 지니는 "주소값"을 그대로 복사해주는 것은 "얕은 복사"
		// --> *m_Array가 지니는 "값"을 가지되, 다른 메모리에 다가 값을 할당하고, 
		// 다른 메모리의 주소값을 지니는 새로운 포인터 변수를 만들어 내는 것은 "깊은 복사"
	}
	/*
	클래스의 멤버 함수
	- 클래스의 멤버 함수는, 일반함수와 달리,
	주소가 하나만 나오게 된다.
	반면, 멤버 변수는, 객체마다 주소가 다르다.
	생성한 객체마다 다른 곳에 잡힌다.

	그런데, 클래스의 멤버함수는,
	모든 instance가 동일한 주소에 있는,
	하나의 멤버함수를 사용한다는 것이다.

	즉, 해당 멤버함수를 호출하는 객체에게
	해당 멤버함수 주소를 가리키는 포인터를
	넘겨주는 원리이다.

	멤버함수를 호출한 객체의 멤버 변수들의 값을,
	정확하게 인식하고 사용할 수 있다.

	그 이유는 this라는 것 때문이다.
	this는 자기 자신에 대한 포인터이다.
	*this는 자기 자신 자체
	this->m_Number1 혹은
	(*this).m_Number1 이렇게 사용하는데,
	앞 부분은 생략할 수 있다.

	this는 이 멤버함수를 호출할 때
	호출하는 객체, instance로 지정이 되게 한다.
	그렇기 때문에, 매번 이 함수를
	호출할 때마다 this에는
	호출하는 객체로 지정이 되는 것이다.

	매번 다른 객체로 호출한다면
	다른 객체들로 매번 this가 바뀔 것이다

`	*/
	
	void Output()
	{
		// cout << "Number1 : " << this->m_Number1 << endl;
		cout << "Number1 : " << m_Number1 << endl;
		// 아래 this는, OutPut의 주소가 아니라, OutPut이라는 함수를 호출하는 객체의 주소가 들어온다
		cout << "Output address : " << this << endl;
	}

	CItem* CPlayer::Equip(Equip_Type Type)
	{
		return nullptr;
	}
	
private:
	// 내부에서만 호출 가능한 함수
	void OutputPrivate() {}
};

int main()
{
	Test test;
	test.x = 1100;

	// CPlayer : 클래스 (틀)
	// player  : object, 객체 (틀을 이용한 제품)
	CPlayer player;
	/*
	아래와 같이 public 는 접근할 수 있다.
	그런데, 이렇게 마구잡이로 바꾸게 하면 문제다.

	즉, 너가 public이라고 붙여주었다는 것은,
	마음대로 바꿔도 됩니다 ! 라고 말해주는 것과 동일하다.

	따라서, 모든 멤버변수는
	되도록이면 protected, private 그리고
	내가 허용한 방법으로만 처리하게 세팅한다.

	그렇게 해야만, 버그가 생길 확률도 줄어들고
	버그가 생겨도, 디버깅이 편하다
	즉, 멤버 변수는 되도록
	public으로 하지 않는다
	*/
	player.m_a = 100;


	// 일반 생성자 호출
	CPlayer* pPlayer = new CPlayer;
	delete pPlayer;

	// int 타입 3개를 인자로 받는 생서자 호출
	CPlayer player1(2020, 2021, 2022);
	player1.Output();
	// int 타입 1개를 인자로 받는 생성자 호출
	CPlayer player2(2020);
	player2.Output();
	CPlayer player3 = player2;

	// player1.Output()함수를 호출하게 되면
	// this는 player1의 주소를 가리키게 된다

	// player2.OutPut()로 호출을 하면
	// this는 player2의 주소를 가리키게 된다. 
	return 0;
}