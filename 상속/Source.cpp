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
	~CParent()
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
public :
	CChild()
	{
		std::cout << "Child 생성자" << std::endl;
	}
	~CChild()
	{
		std::cout << "Child 소멸자" << std::endl;
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
	
	return 0;
}