/*
1) 순수가상함수 형식
2) 순수가상함수 특징 2가지
3) 추상 클래스 정의 및 특징

*/

/*
1) 가상함수 뒤에 =0을 붙여주면
해당 가상함수는 순수가상함수가 된다.

2) 
- 자식 클래스에서 무조건 재정의를 해야 한다.
만약 자식 클래스에서 재정의를 안했다면
해당 자식 클래스는, 추상 클래스가 되어 
해당 클래스 타입의 객체 생성이
불가능하다
- 순수가상함수는 해당 순수가상함수를
만들어주는 클래스에
구현부분을 반들어도 되고, 안들어도 된다.
3) 추상 클래스
- 순수가상함수를 가지고 있는 클래스를
추상클래스라고 한다.

추상클래스는 객체생성이 불가능하다. 
*/

#include<iostream>
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
		cout << "CParent Output" << endl;
	}
	virtual void OutputPure() = 0;
	virtual void OutputPure1() = 0
	{
		cout << "CParent OutputPure1" << endl;
	}
};

class CChild : public CParent
{
public :
	CChild()
	{

	}
	virtual ~CChild()
	{

	}
public :
	virtual void OutputPure() override
	{
		cout << "CChild Outputpure" << endl;
	}
	virtual void OutputPure1() override
	{
		cout << "CChild Outputpure1" << endl;
	}
};

int main()
{
	CParent* Child = new CChild;
	Child->OutputPure();
	delete Child;
}