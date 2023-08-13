#include<iostream>
/*
일반 함수의 정적 변수
- 프로그램 종료될 때까지, 메모리가 유지된다
- 왜냐하면, 메모리의 데이터 영역(전역변수에 같이)에
들어가기 때문이다
ex) stack : 지역변수
ex) heap  : 동적할당

일반 함수 뿐만 아니라,
class 에서도
static 멤버 변수 및 함수를
사용하는 경우가 있다
*/

class CStatic
{
public :
	CStatic(){}
	~CStatic(){}
public :
	int m_iNumber;
	static int m_iNumberStatic;
public :
	void OutPut(){}

	// 1) static 멤버함수에서는
	// this를 사용할 수 없다.
	// 2) 다른 말로 하면, 일반 멤버변수는
	// 사용할 수 없다.
	// 3) 여기에서는 static 멤버변수만
	// 사용이 가능하다
	// 4) 함수 포인터 형태가, 전역함수와
	// 동일한 형태로 사용이 가능하다
	static void OutPutStatic()
	{
		std::cout << "Output Static" << std::endl;
	}
};

// 클래스 static 멤버변수는
// 반드시 클래스 외부에 선언부분을 만들어주어야 한다.
// int의 경우, 기본으로 0으로 초기화되며
// 원하는 것을 대입해주어도 된다.

// 클래스의 static 멤버변수는 이 클래스를 이용해서,
// 객체를 아무리 많이 생성하더라도, 
// 메모리에 딱1개만 생성된다.
// 반면, 일반 멤버변수의 경우, 생성되는 객체의 수만큼 메모리에 잡힌다.
int CStatic::m_iNumberStatic;

// 싱글톤 문제
class CSingleton
{
	CSingleton(){}
	~CSingleton(){}
	CSingleton(const CSingleton&){}
	CSingleton operator=(const CSingleton&){}

	static CSingleton* m_pInst;
public :
	// 멤버 함수 vs 멤버 변수
	// 멤버 변수의 경우, 생성되는 객체마다 다른 메모리에 생성된다
	// 반면, 멤버 함수의 경우
	// 메모리 하나에 위치하여, 하나의 멤버 함수를 모든 객체가 공유하여 쓰는 형태이다

	// 멤버 함수 vs static 멤버 함수
	// static 멤버 함수도, static 멤버 변수와 마찬가지로
	// 생성되는 객체의 수와 관계없이
	// 단 하나만 메모리에 올라가서, 객체들이 공유하는 형태이다
	// 그렇다면, 멤버 함수와의 차이는 무엇일까 ?
	// static 멤버 함수는, this를 사용할 수 없다.
	// 반면 멤버 함수는 this를 사용할 수 있다.
	// this 사용가능 여부에서의 차이가 존재하는 것이다. 
	static CSingleton* GetInst()
	{
		if (m_pInst == nullptr)
		{
			// 포인터에 값을 할당하자
			// 1) 힙 메모리에 CSingleton 구조체 동적할당
			// 2) 해당 주소를 m_pInst 포인터 변수에 저장
			// 3) SingleTon이기 때문에, 객체 하나만을 만들어서 사용
			m_pInst = new CSingleton;
		}
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst != nullptr)
			delete m_pInst;
	}
};
// nullptr은 0
// 함수포인터변수에 0을 넣어준 것과 같은 의미 
CSingleton* CSingleton::m_pInst = nullptr;

int main()
{
	// 1) 접근 방법 : 객체를 통한 접근
	CStatic std1;
	std1.m_iNumberStatic = 100;
	// 2) class를 통한 접근
	CStatic::m_iNumberStatic = 300;

	// 함수 포인터 형태가, 전역함수와 비슷한 방식으로 사용된다. 
	void(*pFunc)() = CStatic::OutPutStatic;
	// 일반 멤버함수의 포인터는 사용방법이 다르다
	void(CStatic:: * pFunc1)() = &CStatic::OutPut;
	// 일반 멤버함수는 반드시 this가 지정되어야 한다
	// 그렇기 때문에 어떤 객체를 this로 지정할지가
	// 반드시 선행되어야 한다
	(std1.*pFunc1)();

	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;

	CSingleton::DestroyInst();

	return 0;
}