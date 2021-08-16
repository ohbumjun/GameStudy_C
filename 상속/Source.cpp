/*
>> 상속의 정의?
>> 상속 : 소멸자 , 생성자
>> Protected ?
>> 상속의 사용 2가지 이유
>> 1) 정의
>> 2) 정의
>> 2) --> 2가지 종류
>> 가상함수와 가상함수 테이블 정의
>> 가상함수 재정의
>> 가상함수와 소멸자
>> 가상함수 재정의 여부
>> 가상함수 호출 과정
>> 멤버변수 없는 클래스,구조체 크기
>> 멤버함수.가 영향주지 않는 이유
*/

/*
>> 상속의 정의 ?
- 클래스는 부모,자식 관계를 형성할 수 있다.
자식 클래스는 부모클래스의 멤버변수 혹은
멤버 함수를 물려받아 사용할 수 있다.

>> 상속관계에서 생성자와 소멸자 호출순서
생성자 : 부모 --> 자식
소멸자 : 자식 --> 부모

>> Protected
클래스의 외부에서는 접근이 불가능하고 ,
클래스의 내부 혹은 자식 클래스의 내부에서는
접근이 가능하다.

>> 상속을 사용하는 이유
1) 재사용성 2) 다형성

>> 1) 재사용성
부모 클래스에
- 공통적으로 가져야할 멤버변수를 선언하고
- 공통적으로 가져야할 기능을 멤버함수로 만들어준 후에
개별적인 내용들만 자식 클래스에 따로
조금씩 만들어서 하나의 객체를 만들 때 사용한다

>> 2) 다형성
상속 관계에 있는 클래스들은
서로 형변환이 가능하다.

다양한 부모와 자식 타입으로의
형변환을 지원해주기 때문에 ,

같은 부모를 상속받고 있는 자식 클래스는,
해당 부모 타입으로
모든 자식 클래스 타압의 변수들을
부모 타입으로 관리가 가능해지게 되는 것이다

>> 업캐스팅 : 자식 --> 부모
>> 다운캐스팅 : 부모 --> 자식

>> 가상함수와 가상함수 테이블 정의

가상함수 : 함수 앞에 virtual을 붙여주기

가상함수를 가지고 있는 클래스를 이용해서
객체를 만들어주게 되면,
가상함수 테이블의 주소를 가지고 있는
포인터 변수를 내부적으로 가지게 된다.

가상함수 테이블은 해당 클래스가
가지고 있는 가상함수의 함수주소를
저장하고 있는 배열이라고 볼 수 있다.

클래스를 이용해서 객체를 생성하면
내부에 __vfptr 포인터 변수가 만들어지고
이 변수에 가상함수 테이블 배열의
메모리 시작 주소가 들어가게 된다.

>> 가상함수 재정의

가상함수는 자식 클래스에서 해당 함수를
재정의할 수 있다.
이를 "함수 오버라이딩"이라고 한다.

함수를 재정의할 때는
함수 반환타입,인자,함수이름 모두 동일해야한다.

함수 재정의이기 때문에 ,
말 그대로 해당 함수를
자식 클래스에서 다시 만들어서,
자식 클래스의 함수가 호출될 수 있도록
만들어주는 기능인 것이다.

>> 가상함수와 소멸자
소멸자는 모든 클래스가 공통되게
이름만 다를 뿐, 소멸자이므로

부모 클래스의 소멸자에
virtual을 붙여주면 ,
자식 클래스는 자동으로 재정의된
소멸자로 인식이 되게 하여
자식 클래스의 소멸자를 호출 -->
해당 자식 클래스를 소멸시킨다.
당연히 이후, 자식 클래스들의
소멸되고 나서, 부모 클래스도 소멸된다.

>> 가상함수 재정의 여부

일반 멤버함수에 virtual을 붙여서
가상함수로 만들어주면 ,
자식 클래스에 재정의를 해도 되고,
안해도 된다.

만약 CChild 클래스는 재정의가 되어 있고
CChild1 클래스는 재정의가 안되어 있으면

가상함수 테이블에 저장되는 함수의 주소는
CChild 클래스를 이용해서 생성한 객체는
CChild에서 재정의된 함수의 주소가,

CChil1 클래스를 이용해서 생성한 객체는
CParent 에서 정의된 함수의 주소가
들어가게 된다.

>> 가상함수 호출 과정
가상함수를 호출하게 되면, 먼저
가상함수인지를 판단하고,

가상함수라면, 가상함수 테이블에서
실제 함수의 주소를 찾아온 이후,
해당 함수의 주소를 이용하여
함수호출을 진행하게 된다.

CChild 클래스를 이용해서 생성한 객체는,
가상함수 테이블에 CChild 클래스에
재정의된 함수의 주소가 들어가 있으므로,
재정의된 함수를 호출해주고

CChild1 클래스를 이용해서 생성한 객체를
가상함수테이블에 CParent 클래스에 선언된
함수의 주소가 들어가 있으므로 ,
해당함수를 호출하게 되는 것이다.

>> 멤버변수 없는 클래스,구조체 크기
struct Test
{
	void hello(){}
}

1byte !
왜냐하면 해당 구조체 및 클래스로
변수를 선언할 수 있는데,
ex) Test test1

이때, 메모리를 할당해주고 변수를 선언하는
원리이므로, 최소한의 메모리를 할당해주기
위해서 1byte를 할당해주는 것이다

>> 멤버함수.가 영향주지 않는 이유
멤버함수의 경우,
코드 영역에 올라가는 것이지,
객체를 생성할 때,
메모리에 잡히는 것은
아니기 때문이다.

*/

#include<iostream>
using namespace std;

class CParent
{
public:
	CParent()
	{
		cout << "CParent 생성자" << endl;
	}
	virtual ~CParent()
	{
		cout << "CParent 소멸자" << endl;
	}
private:
	int m_Number1;
protected:
	int m_Number2;
public:
	int m_Number3;
public :
	int m_Number4;
public:
	virtual void Output()
	{
		cout << "CParent Output" << endl;
	}
};

class CParent1
{
public:
	CParent1()
	{
		cout << "CParent1 생성자" << endl;
	}
	virtual ~CParent1()
	{
		cout << "CParent1 소멸자" << endl;
	}
public:
	int m_Number3;
public:
	int m_Number5;
};

class CChild : public CParent
{
public:
	CChild()
	{
		cout << "CChild 생성자" << endl;
		// CParent
		m_Number4 = 100;
	}
	~CChild()
	{
		cout << "CChild 소멸자" << endl;
	}
public:
	void Output()
	{
		cout << "CChild Output 함수" << endl;
	}
};

class CChild1 : public CParent, CParent1
{
public:
	CChild1()
	{
		cout << "CChild1 생성자" << endl;
		// CParent 
		m_Number4 = 500;
	}
	~CChild1()
	{
		cout << "CChild1 소멸자" << endl;
	}
};

int main()
{
	// 1) 크기 ?
	cout << sizeof(CChild1) << endl;;
	// 2) 멤버 변수 상속 원리
	CChild* child   = new CChild;
	cout << child->m_Number4 << endl;
	CChild1* child1 = new CChild1;
	cout << child1->m_Number4 << endl;
	cout << child->m_Number4 << endl;

	// 3) 다중 상속의 위험성
	// cout << child1->m_Number3 << endl;

	// 4) 다형성 
	CParent* child2 = new CChild;
	CParent* child3 = new CChild1;

	// 5) 다운 캐스팅의 위험성 
	((CChild1*)child2)->Output();

}

/*
1) 32
>> CParent : 멤버변수 4개 + virtual 
가상함수 테이블 주소 가리키는 포인터 1개
>> CParent1 : 멤버변수 2개 + virtual
가상함수 테이블 주소 가리키는 포인터 1개

총 7 * 4 == 28

2) 부모 클래스에 정의된 멤벼 변수에 대해
각각 별도의 메모리 공간을 할당하여
상속받는다.

즉, CChild와 CChild1 클래스의 경우,
둘다 CParent를 상속받고
m_Number4 라는 멤버변수를 상속받는다.

이때, 같은 메모리 공간에 있는
m_Number4라는 변수를 공유하는 개념이 아니다

CChild, CChil1 각각, 객체를 생성시
m_Number4 라는 각각의 변수를
각각의 서로 다른 메모리 공간에 할당하여
사용하는 원리이다. 

3) CChild1의 경우 ,
CParent, CParent1을 다중 상속받는 중인데 ,
CParent, CParent1의 경우, 
공통적으로 m_Number3 라는 변수를 갖고 있다.
따라서, 이를 cout << endl 하려할 때
두 부모 class 중 어떤 것의 m_Number3를 출력해야 하는지 모르게 된다.
즉, 변수의중복을 조심해야 한다. 

4) CChild 클래스를 동적할당하고,
해당 메모리 주소를 CParent 포인터 타입에 저장.
서로 상속관계에 있는 클래스이기 때문에
이러한 형변환이 가능한 것이다.

5) child3는 근본적으로 CParent 포인터 타입의 변수.
다운 캐스팅을 할시 주의해서 사용.

CChild 생성 --> CParent* 형변환 --> CChild1 형변환

이 객체는 근본적으로 CChild 객체를 생성한 것이므로
CChil1 으로 형변환 하여 사용시 
많은 문제를 발생시킬 수 있다. 

*/