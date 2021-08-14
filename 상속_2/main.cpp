#include<iostream>

/*
-------
추상 클래스 : 순수가상함수를 가지고 있는
클래스를 추상클래스라고 한다.

-------
순수가상함수 :
가상함수 뒤에 = 0을 붙여주면
해당 가상함수는 순수가상함수가 된다.

순수가상함수는 자식클래스에서 무조건
재정의를 해야 한다

만약 자식 클래스에서 재정의를 안한다면
해당 자식 클래스는 추상클래스가 되어서
해당 클래스 타입의 객체 생성이 불가능하다.

-------
순수가상함수는 해당 순수가상함수를 만들어주는
클래스에 구현부분을 만들어도 되고
안만들어도 된다.

순수가상함수는 자식클래스마다 따로 구현이 되어야 하는 경우
순수가상함수로 만들어서
자식에서 무조건 재정의를 하여
구현하게끔 만들 때 사용한다. 

*/

using namespace std;

class CParent
{
public :
	CParent()
	{

	}
	virtual ~CParent()
	{

	}
public :
	virtual void Output()
	{

	}
	// 순수가상함수
	virtual void OutputPure() = 0;

	// 구현부분이 있을 수도 있고 없을 수도 있다
	// 다만, 순수가상함수를 정의하는 순간
	// CParent는 추상 클래스가 되고
	// CParent를 통해 객체를 생성할 수 없게 된다.
	virtual void OutputPure1() = 0
	{
		cout << "CParent OuptutPure1" << endl;
	}
};

class CChild : public CParent
{
public:
	CChild()
	{
	}
	virtual ~CChild()
	{
	}
public:
	// 일반 가상함수 : 자식 클래스에서
	// 재정의를 해도 그만, 안해도 그만
	virtual void Output()
	{
	}
	// 다만, 순수가상함수는 무조건 재정의
	virtual void OutputPure() = 0;

	virtual void OutputPure1() = 0
	{
	}
};


class CChild1 : public CParent
{
public:
	CChild1()
	{

	}
	virtual ~CChild1()
	{

	}
public:
	// 일반 가상함수 : 자식 클래스에서
	// 재정의를 해도 그만, 안해도 그만
	// virtual void Output()

	virtual void OutputPure() = 0;

	virtual void OutputPure1() = 0
	{
	}
};


int main()
{


	return 0;
}