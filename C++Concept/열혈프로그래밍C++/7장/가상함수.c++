// Q. 아래가 컴파일 에러가 나는 이유 
class Base
{
public :
    void BaseFunc(){}
};

class Derived : public Base 
{
public :
    void DerivedFunc(){}
};

int main()
{
    Base* bptr = new Derived;
    bptr->DerivedFunc();
    // 컴파일 에러
}

// 왜냐하면, bptr이 Base형 포인터이기 때문이다.
// 그래도, 실제로 해당 포인터가 가리키는 객체는 Derived 객체니까 가능해야 하니 않나요
// 아니다. C++ 컴파일러는, 포인터 연산의 가능성 여부를 판단할 때, 포인터의 자료형을 기준으로 판단하지
// 실제 가리키는 객체의 자료형을 기준으로 판단하지 않기 때문이다. 

// 즉, "포인터 형에 해당하는 클래스에 정의된 멤버에만 접근이 가능한 것이다"

// Q. 아래 코드가 컴파일 나는 이유                              ------
int main()
{
    Base* bptr = new Derived();
    Derived * dptr = bptr;
}

// Derived 클래스는, Base 클래스의 유도 클래스이니까
// Base 클래스의 포인터 변수로 Derived 객체의 참조는 가능하다 ( 첫번째 )

// (2번째 문장) 이제 bptr은 실제로 가리키는 객체가 Derived 객체라는 사실을 기억하지 않는다
// bptr은 Base형 포인터 이니까, bptr이 가리키는 대상은 Base객체일 수 있다
// 이 경우, 2번째 문장이 성립하지 않으므로, 컴파일 에러를 발생시키자

// Q. 아래 코드가 정상적으로 컴파일 나지 않는 이유              -----
int main()
{
    Derived* dptr = new Derived;
    Base* bptr = dptr;
}

// dptr은 Derived 클래스의 포인터 변수니까
// 이 포인터가 가리키는 객체는, 분면 Base 클래스를 직접 혹은 간접적으로 상속하는 객체이다
// 그러니 Base 형 포인터 변수로도 참조가 가능하다. 

// Q. 가상함수의 특징 2 + 아래 코드 결과 이유
class First
{
public :
    virtual void func() {cout << "First" << endl;}
};

class Second
{
public :
    virtual void func() {cout << "Second" << endl;}
};

class Third
{
public :
    virtual void func() {cout << "Third" << endl;}
};

int main()
{
    Third* tptr = new Third();
    Second* sptr = tptr;
    First* fptr = sptr;

    fptr->func(); // Third 
    sptr->func(); // Third
    tptr->func(); // Third
    return 0;
}


// 가상함수의 선언은 virtual 키워드의 선언을 통해 이루어진다.
// 이렇게 가상함수가 선언되고 나면, 이 함수를 오버라이딩 하는 함수도 가상함수가 된다.

// 함수가 가상함수로 선언되면, 해당 함수 호출 시, 포인터의 자료형을 기반으로 호출대상을 결정하지 않고
// 포인터 변수가 실제로 가치키는 객체를 참조하여, 호출의 대상을 결정한다.

// Q. 상속을 하는 이유 ?                                                ------
// - 상속을 통해 연관된 일련의 클래스에 대해 공통적인 규약을 정의할 수 있다. 

// Q. 순수 가상 함수, 추상클래스 정의 , 취지                               ------

// 순수 가상함수란, 함수의 몸체가 정의되지 않은 함수.를 의미한다. 이를 표현하기 위해 = 0 을 표시한다.
// 추상 클래스란, 하나 이상의 멤버함수를, 순수 가상함수로 선언한 클래스.이다. 이는 객체 생성이 불가능한 클래스가 된다. 

// 클래스 중에서는, 기초 클래스로서의 의미만 지닐 뿐
// 객체 생성을 목적으로 정의되지 않은 클래스도 존재하므로, 하나의 멤버함수를 가상하수로 세팅하여 
// 추상 클래스로 만든다.


// Q. 다형성 ?                                                          -----
// 가상함수의 호출 관계에서 보인 특성, 
// 즉, 해당 함수 호출 시, 포인터의 자료형을 기반으로 호출대상을 결정하지 않고
// 포인터 변수가 실제로 가리키는 객체를 참조하여, 호출의 대상을 결정하는 형태

// 문장은 같은데 , 결과는 다른 특성

// Q. 아래 코드. 함수들의 실행 결과가 다른 이유                           -----
class First
{
public :
    virtual void func()
    {
        cout << "first" << endl;
    }
};

class Second : public First
{
public :
    virtual void func()
    {
        cout << "second" << endl;
    }
};

int main()
{
    First* ptr = new First(); // first 
    ptr->func();
    delete ptr;

    ptr = new Second(); // second 
    ptr->func();
    delete ptr;

    return 0;
}

// ptr은 동일한 포인터 변수, 그런데 결과는 다르다. 
/// 즉, 포인터의 자료형은 동일하지만 
// 포인터 변수 ptr 이 참조하는 객체. 포인터 변수가 실제로 가리키는 객체의 자료형이 다르기 때문이다.

// < 가상 소멸자 >
// Q. 아래 코드 결과    , 해결책, 해결되는 이유                ---
class First
{
public :
    char* strOne;
public :
    First()
    {
        strOne = new char[5];
    }
    ~First()
    {
        delete [] strOne;
    }
};

class Sccond : public First
{
public :
    char* strTwo;
public :
    Sccond()
    {
        strTwo = new char[5];
    }
    ~Sccond()
    {
        delete [] strTwo;
    }
};

int main()
{
    First* ptr = new Second;
    delete ptr;
    return 0;
}

// 메모리 leak이 발생한다
// 객체의 소멸을 First 형 포인터로 명령하기 때문에 , First 클래스의 소멸자만 호출된다.
// Second 클래스의 소멸자는 호출되지 않고, 결국 strTwo 변수가 메모리에 남게 되는 것이다. 
// 정리하자면, new로 할당한 클래스는 유도 클래스
// 해당 클래스를 기본 클래스 First 포인터를 통해 삭제하고 있고
// 기본 클래스의 소멸자가 바기상 소멸자일 경우, 기본 클래스의 소멸자만 호출될 뿐, 유도 클래스의 소멸자는 호출되지 않는다 .

// 따라서, 객체 소멸과정에서는 delete 연산자에 사용된 포인터 변수의 자료형에 관계없이, 모든 소멸자가 호출되어야 한다.
// 이를 위해 First 클래스의 소멸자를 virtaul로 선언해준다.
virtual ~First()
{
    delete [] strOne;
}

// 이유 ---
// 소멸자도, 상속의 계층구조상 맨 위에 존재하는 기초 클래스의 소멸자만 virtual로 선언하면 
// 이를 상속하는 유도 클래스의 소멸자들도 모두 "가상 소멸자"로 선언이 된다.
// 그리고 가상 소멸자가 호출되면, 상속의 계층 구조상 맨 아래에 존재하는 유도 클래스의 소멸자가 대신 호출되면서 (현재 제거하는 유도클래스부터 )
// 기초 클래스의 소멸자가 순차적으로 호출된다( 아래에서 위로 올라오면서 )


// Q. 참조자와 객체 사이의 특성 2 
// 1) C++ 에서, AAA형 참조자는, AAA객체 또는 AAA를 직접 혹은 간접적으로 상속하는 모든 객체를 참조할 수 있다. 
// 2) First 형 참조자를 이용하면, First 클래스에 정의된 func() 함수 실행, 
//    Second 형 참조자를 이용하면, Second 클래스에 정의된 func() 함수 실행 

// Q. 아래 코드 호출 결과 ? 
#include <iostream>

using namespace std;

class First
{
public :
	virtual void func()
	{
		cout << "first" << endl;
	}
};

class Second : public First
{
public :
	virtual void func()
	{
		cout << "second" << endl;
	}
};

class Third : public Second //
{
public :
	virtual void func()
	{
		cout << "third" << endl;
	}
    virtual void func3()
	{
		cout << "third3" << endl;
	}
};

int main()
{
	Third obj;
    obj.func();

    Second &sref = obj;
    sref.func();

    First &fref = obj;
    fref.func3();
    fref.func();
}

// third
// third
// 컴파일 에러  //////
// third

// 컴파일러는, 참조자의 자료형을 가지고, 함수의 호출가능성을 판단한다.
// 단, 호출하려는 함수가 가상함수일 경우, 해당 참조자가 가리키고 있는 객체에 정의된 함수를 호출한다.

// First &fref = obj; 
// First 클래스에 정의된 함수만 호출 가능하다.
// fref.func3(); --> 컴파일 에러가 나는 것이다. First Class에 정의된 함수가 아니기 때문이다 
// fref.func(); --> func는 가상함수 이므로, Third 클래스에서 정의된 func 함수가 호출된다.

// Q. 아래 코드에서 유추할 수 있는 2가지 특성
void GoodFunction(const First& ref);

// First 객체 또는 First를 직접 혹은 간접적으로 상속하는 클래스의 객체가 인자의 대상이 되겠구나
// 인자로 전달되는 객체의 실제 자료형에 상관없이 함수 내에서는 First 클래스에 정의된 함수만 호출가능하다. 
