
#include<iostream>

using namespace std;

class Car
{
private: 
    int fuelGauge;
public :
    Car(int fuel) : fuelGauge(fuel){}
    void ShowCarState(){cout << "잔여 연료량 : " << fuelGauge << endl;}
};

class Truck : public Car
{
private :
    int freightWeight;
public :
    Truck(int fuel, int weight) : Car(fuel), freightWeight(weight)
    {}
    void ShowTruckState()
    {
        ShowCarState();
        cout << "화물 무게 : " << freightWeight << endl;
    }
};

int main()
{
    Car* pcar1 = new Truck(80, 200);
    Truck* ptruck1 = (Truck*)pcar1;
    ptruck1->ShowCarState();
    cout << endl;

    Car* pcar2 = new Car(120);
    Truck* ptruck2 = (Truck*)pcar2;
    ptruck2->ShowTruckState();
    cout << endl;

    return 0;
}

// Q. 위 코드 결과 이상한 결과가 나오는 점 + 그 이유

// ptruck2->ShowCarState(); 에서 freightWeight 가 쓰레기값이 나올 것이다.
// 왜냐하면, 기본 클래스의 포인터 형을 유도 클래스의 포인터 형으로 형변환하는 것은
// 일반적인 경우의 형 변환이 아니다.

// 위의 코드가 의도한 것일 수도 있고, 실수일 수도 있다
// Q. 용도에 맞는 형변환을 지원해주기 위한 4가지 형변환 종류  ---
// static_cast, dynamic_cast, const_cast, reinterpret_cast 

// Q. static_cast의 용도

// 기본 클래스 포인터 형을 유도 클래스 포인터형으로 형변환 시켜주지만, 책임은 네가 져라.
// 하지만, 위의 경우에서도 봤듯이, static_cast는 최대한 제한적 사용
// 즉, 기본 클래스 포인터 혹은 참조형 데이터를, 유도 클래스 포인터 혹은 참조형 데이터로 변환 

// Q. dynamic_cast 용도 2가지

// - 1) 유도 클래스 포인터 혹은 참조형 데이터를 기초 클래스 포인터 및 참조형 데이터로 변환하는 경우

#include <iostream>

using namespace std;

class Car
{
};

class Truck : public Car
{
};

int main()
{
    Car* pcar1 = new Truck(40, 100);
    Truck* ptruck1 = dynamic_cast<Truck*>(pcar1); // 컴파일 에러 

    Car* pcar2 = new Car(40, 100);
    Truck* ptruck2 = dynamic_cast<Truck*>(pcar2); // 컴파일 에러 
    
    Truck* pcar3 = new Truck(40, 100);
    Car* ptruck3 = dynamic_cast<Car*>(pcar3); // OK

    return 0;
}

// - 2) 기본 클래스 포인터 혹은 참조형 데이터를, 유도 클래스 포인터 혹은 참조형 데이터로 변환  (static_cast와 동일)

// 단, 기초 클래스가 Polymorphic 클래스일 때 == 하나 이상의 가상 함수를 지닐 때
// 즉, 상속 관계에 놓인 두 클래스 사이에서, 기본 클래스가 하나 이상의 가상함수를 지닌다면
// dynamic_cast를 통해, 기본 클래스 포인터 혹은 참조형 데이터를
// 유도 클래스 포인터 혹은 참조형 데이터로 바꿔줄 수 있다.

#include<iostream>

using namespace std;

class Simple
{
public :
	virtual void ShowInfo()
	{
		cout << "Simple" << endl;
	}
};

class Complex : public Simple
{
public:
	virtual void ShowInfo() override
	{
		cout << "Complex" << endl;
	}
};

int main()
{
	Simple* simPtr = new Complex;
	Complex* comPtr = dynamic_cast<Complex*>(simPtr);

	comPtr->ShowInfo();
	return 0;
}

// Q. const_cast 사용용도 ---

// 포인터와 참조자의 const 성향을 제거하는 형변환을 목적으로 사용한다.
#include<iostream>

using namespace std;

void ShowString(char* str)
{
	cout << str << endl;
}

int main()
{
	const char* name = "bum jun";

	ShowString(const_cast<char*>(name));

	return 0;
}

// Q. reinterpret_cast + 유용하게 사용되는 예시 ( 이유 )

// 전혀 상관없는 자료형으로의 형변환에 사용된다. 
int main()
{
    Car* car = new Car;
    Friend* friend = reinterpret_cast<Friend*>(car);
}

// int형 정수에 바이트 단위 접근 하기 위해서
int main()
{
    int num = 0x01020304;
    char* ptr = reinterpret_cast<char*>(&num); // 참조형 

    for (int i = 0; i < sizeof(num); i++)
    {
        printf("address : %p , value : %X \n", ptr + i , *(ptr + i));
        cout << endl;
    }
}

// 리틀 앤디안 ---
// Big Endian과 Little Endian은 컴퓨터 메모리에 저장된 바이트들의 순서를 설명하는 용어
/*
address : 000000F1AD9BFB94 , value : 4
address : 000000F1AD9BFB95 , value : 3
address : 000000F1AD9BFB96 , value : 2
address : 000000F1AD9BFB97 , value : 1

ex) 우리가 1492 라는 수를 기록할 때
리틀 엔디안 방식의 경우 , 2941 이라고 기록하고
2941을 읽을 때는, 1492라고 읽는 방식 

즉, 데이터를 읽거나, 메모리에 저장할 때
하위 바이트를 먼저 읽거나, 하위 바이트가 먼저 저장된다는 것을 의미한다.
*/


// Q. 기본형 -> 유도형 : dynamic_cast, static_cast 차이점 2개

// 1) dynamic_cast의 조건은, 기본형 포인터가 가리키는 객체가 유도 클래스 객체이어야 한다는 점이다. 
class Simple
{
public :
	virtual void ShowInfo()
	{
		cout << "Simple" << endl;
	}
};

class Complex : public Simple
{
public:
	virtual void ShowInfo() override
	{
		cout << "Complex" << endl;
	}
};

int main()
{
	Simple* simPtr = new Complex; // 실제 가리키는 객체가 애초부터 Complex 객체였으므로 
	Complex* comPtr = dynamic_cast<Complex*>(simPtr); // Complex 형 포인터로 Complext 객체를 가리켜도 되는 것 

	comPtr->ShowInfo();
	return 0;
}

// Simple* simPtr = "new Simple"; 라고 수정하면, 결과는 Null 이 return 된다. 

// 2) dynamic_cast 의 경우, 컴파일 시간이 아니라, 실행 시간 (프로그램이 실행 중인 동안에)
// 안전성을 검사하도록 컴파일러가 바이너리 코드를 생성한다
// 이로 인해, 실행 속도는 늦어지지만, 안정적인 형변환이 가능

// 반면, static_cast는 안전성을 보장하지 않는다.
// 컴파일러는 무조건 형변환이 되도록, 바이너리 코드를 생성하기 때문에
// 그로 인한 실행의 겨로가는 전적으로 프로그래머
// 즉, 실행 중인 동안에는 안전성 검사를 하지 않아서 static 


// Q. dynamic_cast ~ bad_cast 예시

// bad_Cast : 프로그래머가 정의하지 않아도 발생하는 예외 -> 형 변환시 발생하는 bad_cast

#include <iostream>
#include <new>

using namespace std;

class Simple
{
public:
	virtual void ShowInfo()
	{
		cout << "Simple" << endl;
	}
};

class Complex : public Simple
{
public:
	virtual void ShowInfo() override
	{
		cout << "Complex" << endl;
	}
};

int main()
{
	Simple simObj;
	Simple& ref = simObj;

	try
	{
		Complex& comRef = dynamic_cast<Complex&>(ref);
		comRef.ShowInfo();
	}
	catch (bad_cast expt)
	{
		cout << expt.what() << endl;
	}
	
	return 0;
}