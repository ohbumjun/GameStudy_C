// 가상함수의 호출 관계에서 보인 특성, 
// 즉, 해당 함수 호출 시, 함수가 가상함수라면 포인터의 자료형을 기반으로 호출대상을 결정하지 않고
// 포인터 변수가 실제로 가리키는 객체를 참조하여, 호출의 대상을 결정하는 형태

// 문장은 같은데 , 결과는 다른 특성
// 다형성이란 하나의 타입에 여러 객체를 대입할 수 있는 성질
#include<iostream>

using namespace std;

class First
{
public :
    virtual void func()
	{
		cout << "First" << endl;
	}
};

class Second : public First
{
public :
    void func()
    {
    	cout << "Second" << endl;
	}
};

class Third : public Second 
{
public :
    void func()
    {
    	cout << "Third" << endl;
	}
};

int main()
{
    First* fptr = new First;
    fptr->func();
    delete fptr;
    
    fptr = new Second;
    fptr->func();
    delete fptr;

    fptr = new Third;
    fptr->func();
}
