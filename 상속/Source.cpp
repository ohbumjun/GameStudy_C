#include<iostream>

/*
상속 :
- 클래스는, 부모와 자식의 관계를
형성할 수 있다.

자식 클래스는 부모클래스의 멤버변수 혹은
멤버함수를 물려받아 사용할 수 있게 된다.


상속관계에서 생성자와 소멸자의 호출 순서
생성자 : 부모 --> 자식
소멸자 : 자식 --> 부모 

protected :클래스의 외부에서는
접근이 불가능하다.
클래스의 내부 혹은 자식 클래스의
내부에서는 접근이 가능하다.

private : 자식클래스에서 조차도
접근이 불가능하다. 

보통 상속을 사용하는 이유는
클래스의 재사용성도 하나의 이유가 된다.

부모클래스에 공통적으로 가져야할
멤버변수를 선언하고
공통으로 가져야할 기능을
멤버함수로 만들어준 이후에 ,

개별적인 내용들만 자식클래스에 따로
조금씩 만들어서, 
하나의 객체를 만들때 사용한다. 


-----------------------------
상속관계에 있는 클래스들은
서로 형변환이 가능하다
자식 -> 부모
부모 -> 자식

다형성 ?
다양한 부모와 자식 타입으로의
형변환을 지원해주기 때문에
같은 부모를 상속받고 있는
자식 클래스는 해당 부모타입으로
모든 자식 클래스 타입의 변수들을
부모타입으로 관리가 가능해지게 되는 것이다

업캐스팅 : 자식을 부모 타입으로 형변환
다운 캐스팅 :  부모를 자식 타입으로 형변환 

가상함수 ?
- 함수 앞에 virtual을 붙여서
가상함수를 만들어줄 수 있다

가상함수를 가지고 있는 클래스를 이용해서
객체를 생성하면, 

가상함수 Table의 주소를 가지고 있는
포인터 변수를 내부적으로 
가지게 된다. 

가상함수 테이블이란 ?
해당 클래스가 가지고 있는
가상함수의 함수주소를 가지고 있는
배열이라고 볼 수 있다

클래스를 이용해서 객체를 생성하면
내부에 __vfptr포인터 변수가 만들어지고,
이 변수에 가상함수테이블 배열의 
메모리 시작 주소가 들어가게 된다.


가상함수는 자식클래스에서 
해당 함수를 재정의 할 수 있다.

이를 함수 오버라이딩이라고 한다.

함수를 재정의 할 때는, 반환타입,
인자가 모두 동일해야 한다.

함수 이름도 당연히 동일해야 한다.
함수 재정의이기 때문에
말 그대로 해당 함수를 자식 클래스에
다시 만들어서 자식 클래스의 함수가
호출될 수 있도록 만들어주는 기능인 것이다.

소멸자는 모든 클래스가 공통되게 이름만 다를 뿐
소멸자 이므로, 
부모클래스의 소멸자에 virtual을 붙여주면
자식클래스는 자동으로 재정의된
소멸자로 인식이 되게 된다. 
즉 , virtual 붙이면, 무조건 재정의 되는 것

일반 멤버함수에 virtual을 붙여서
가상함수를 만들어주면
자식 클래스의 재정의를 해도 되고
안해도 된다.

만약 CChild클래스는 재정의가 되어 있고
CChild1 클래스는 재정의가 안되었을 경우

가상함수 테이블에 저장되는 함수의 주소는
CChild 클래스를 이용해서 생성한 객체는
CChild 클래스에 재정의한 함수의 주소가 들어가게 되고

CChild1 클래스를 이용해서 생성한 객체는
재정의가 안되어 있으므로
CParent 클래스에 정의된 함수의 주소가
들어가게 되는 것이다

그래서 가상함수를 호출을 하게 되면
먼저 가상함수인자를 판단하고 
가상함수라면 가상함수 테이블에서

실제 함수의 주소를 찾아온 후에 
해당 함수의 주소를 이용하여 
함수호출을 진행하게 되므로

CChild 클래스를 이용해서 생성한 객체는
가상함수테이블에 CChild 클래스에
재정의된 함수의 주소가 들어가 있으므로
재정의된 함수를 호출해주고

CChild1 클래스를 이용해서 생성한 객체는
가상함수 테이블에 CParent 클래스에
선언된 함수의 주소가 들어가 있으므로
해당함수를 호출하게 되는 것이다. 

ChildChild 와 같이 한번 더 상속된 애의 경우
중간에 만약 재정의를 했다면,
할아버지가 아니라, 부모 가상함수 정보가
내려온다 ( 이건 내가 직접 확인해보기 )


만일  , 재정의한 함수에서
부모 함수를 호출하고 싶다면 

CChild->CParent::Output();
// 이렇게 명시해주면 된다.



그런데 만일
자식에만 정의된 함수를 정의하려면 ?
CParnet* Child = new CChild'

Child는 근본적으로 CParent* 타입의 변수이다.
즉, CHild 주소는 담지만,
타입은 CParent라는 것이다

따라서, 자식 클래스에만 있는 함수를
호출하려면

형변환을 해주어야 한다

((CChild*)Child)->ChildOutput();

// 하지만
다운캐스팅을 할 때는 정말 주의해서 사용
왜냐하면 Child는 CChild 타입 클래스를
객체로 생성한 것이다.

그런데 만일 이것을 CCHild1 타입으로 형변환 하면
CChild생성 -> CParent 형변환 -> CChild1 형변환이 되어버리는 것
// 이 객체는 근본적으로 CChild객체를 생성한 것이므로
CChild1 타입으로 형변환해서 사용할 경우
많은 문제를 발생시킬 수 있다 . 

// 즉, 생성한 객체에 맞는 다운캐스팅이 아니라
다른 것으로 하면 xx 

*/

struct Test
{
public:
	void Output()
	{

	}
};

class CParent
{
public :
	CParent()
	{
		std::cout << "Parent 생성자" << std::endl;
	}
	// 생성자 x 
	// 소멸자에 가상함수를 붙여줄 수 있다
	// 단, 상속구조에서
	// 부모 소멸자에 virtual을 붙여야 한다
	// 부모에 안붙이고, 자식에게만 붙이면 x 
	virtual ~CParent()
	{
		std::cout << "Parent 소멸자" << std::endl;
	}
public :
	int m_Number1;
private : // private : 자식 클래스에서도 접근이 불가능하다.
	int m_Number2;
protected :
	int m_Number3;
public :
	void Output()
	{

	}
};

class CChild : public CParent
{
private:
	int* m_NumberPointer;
public :
	CChild()
	{
		std::cout << "Child 생성자" << std::endl;
		m_NumberPointer = new int;
	}
	~CChild()
	{
		std::cout << "Child 소멸자" << std::endl;
		delete m_NumberPointer;
	}
};

// private 상속 ?
class CChild1 : private CParent
{
public:
	CChild1()
	{
		std::cout << "Child1 생성자" << std::endl;
		
		m_Number1 = 100;
		m_Number3 = 300;
	}
	~CChild1()
	{
		std::cout << "Child1 소멸자" << std::endl;
	}
};

// 비록 CChild에서 public으로 받아도
// 중간에 CChild에서 private으로 한번 걸러버림
// 따라서 CParent의 public 변수에 접근할 수 없게 된다.
class CChildChild1 : public CChild
{
public:
	CChildChild1()
	{
		std::cout << "CChildChild1 생성자" << std::endl;
	}
	~CChildChild1()
	{
		std::cout << "CChildChild1 소멸자" << std::endl;
	}
};

// ------------------------------------------------------------
// 다중 상속 


class CParent1
{
public:
	CParent1()
	{
		std::cout << "Parent 생성자" << std::endl;
	}
	~CParent1()
	{
		std::cout << "Parent 소멸자" << std::endl;
	}

// 조심해야 하는 부분이다.
// m_Number3 이 CParent, CParent1 둘다
// m_Number3을 지니고 있기 때문이다. 
// 다중상속일 때, 멤버변수가 겹치게 하면
// 안된다는 것이다.
public:
	int m_Number3;
public:
	int m_Number4;

};

class CChild2 : public CParent, CParent1
{
public:
	CChild2()
	{
		std::cout << "Child2 생성자" << std::endl;
	}
	~CChild2()
	{
		std::cout << "Child2 소멸자" << std::endl;
	}
};


// 다형성 ---------------------------------
// 상속을 안쓴다고 가정하면
// player, monstr는 무조건 다른 class가 되어야 한다


class CTestParent
{
public :
	CTestParent()
	{

	}
	virtual ~CTestParent()
	{

	}
};

class CTestChild : public CTestParent
{
public:
	CTestChild()
	{

	}
	~CTestChild()
	{

	}
};

class CTestParent1
{
public:
	CTestParent1()
	{

	}
	virtual ~CTestParent1()
	{

	}
	virtual void Output()
	{

	}
private :
	int m_Number;
};

class CTestChild1 : public CTestParent
{
public:
	CTestChild1()
	{

	}
	~CTestChild1()
	{

	}
};


int main()
{
	// 멤버 변수가 없을 경우 
	// 구조체, class 의 크기는 1byte가 나온다.
	// 왜 1byte가 나오는 것일까 ?
	// 만약 멤버변수가 없다고 하여, 크기가 아예 없으면
	// 주소공간 자체가 할당될 수 없다
	// 왜 ? 아래와 같이 instance, 변수를 만드려면
	// 메모리가 할당되어야 하는데, 이를 위해서
	// 최소한의 메모리를 할당해야 된다는 것이다
	// 즉, 구조체나 클래스는 해당 타입을 이용하여
	// 변수를 선언할 수 있기 때문에
	// 아래와 같은 과정들이 가능한 것이다.
	// "멤버변수"를 가져야만 한다.
	// "멤버변수" 가 있어야 추가 메모리가 할당
	// "멤버함수"만 존재하더라도 여전히 1byte
	// 해당 "멤버함수"안에 지역변수가 있더라도 여전히 1byte
	// 코드영역에 올라가는 것이지.
	// 객체를 생성할 때, 메모리가 잡히는 것은 아니기 때문이다
	Test test1;
	std::cout << sizeof(CParent) << std::endl;
	std::cout << sizeof(Test) << std::endl;
	
	CChild child;
	CChild1 child1;

	// 아무리 public 변수라고 하더라도
	// 상속을 private으로 받아버리면
	// 접근 불가능하다 
	// child1.m_Number1 = 300;



	// 다중 상속
	// 2개의 부모 생성자 먼저 --> 그 다음 CChild2 생성자
	// 이후, 소멸자도, CChild2 이후, Parent 2개 연속 
	CChild2 child;


	// 다형성
	// ex) class를 동적할당
	// CChild 클래스를 동적할당하고
	// 해당 메모리주소를 CParent 포인터 타입에 저장해둔다
	// 서로 상속관계에 있는 클래스이기 때문에
	// 이런 형변환이 가능한 것이다. 
	CParent* Child = new CChild;
	
	// 단, private 상속은, 불가능하다 ! 
	// CParent* Child1 = new CChild1;

	// 자식 -> 자식 도 가능하다 !
	CParent* ChildChild1 = new CChildChild1;

	// 아래의 경우
	// 아무런 조치도 취해주지 않을 경우 
	// Child, ChildChild1 각각의 소멸자를 건너뛰고
	// Parent 소멸자만 호출되는 것을 확인할 수 있다.
	
	// 단. parent class를 virtual로 만들어주면
	// 자식 클래스에 대한 소멸자도 호출된다.
	// 왜 ?? 소멸자는 공통된 함수라고 봐야하기 때문이다.
	// 무슨 말이여 ?
	// 
	delete Child;
	delete ChildChild1;



	// 가상함수 size
	// 32bit, 64bit 에 따라 변하는 것은
	// 포인터의 크기

	// 비어있는 경우라고 하더라도
	// 1byte 가 아니라, 4byte가 나온다.
	std::cout << sizeof(CTestChild) << std::endl;

	// 아래의 경우도 마찬가지
	// 내부적으로, 가상함수 테이블에 대한 포인터를 가지고 있으므로
	// 그 포인터변수에 대한 크기만큼을 
	// 내부적으로 가지고 있게 되는 것이다
	// 뿐만 아니라, virtual 함수 여러개라면
	// 여러개 virtual 함수만큼
	// 달려있는 포인터변수도 개수도 증가하게 된다.
	std::cout << sizeof(CTestChild1) << std::endl;

	return 0;
}