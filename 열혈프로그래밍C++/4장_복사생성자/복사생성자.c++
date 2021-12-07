// Q. 복사 생성자 3가지 특성 ---

// - 멤버 대 멤버의 복사가 이러워지는 생성자 
// - const를 사용하여, 복사에 사용되는 원본을 변경시키는 것을 막는다. ex) Simple(const Simple& simple){}
// - 복사 생성자를 정의하지 않으면, "멤버 대 멤버의 복사를 진행하는" 디폴트 복사 생성자가 자동호출된다.

// Q. 복사 생성자 막는 방법

// - explicit 키워드를 사용한다
// - Simple sim2 = sim1; --> 복사 생성자 묵시적 호출 : 묵시적 호출 막기 위해서는 explicit 사용
// ex) explicit Simple(const Simple& copy){}
// 이렇게 하면, 더이상 묵시적 변환이 발생하지 않아서, 대입 연산자를 이용한 객체의 생성 및 초기화가 불가능해진다.

// Q. 복사 생성자 호출 시점 3가지 , 예시 , 3번째 이유 

// - 1) 기존에 생성된 객체를 이용해서 새로운 객체를 초기화 하려는 경우
// ex) Person man2 = man1;
// - 2) Call - by -value 방식의 함수 호출 과정에서, 객체를 인자로 전달하는 경우
// - 3) 객체를 반한하되, 참조형으로 반환하지 않는 경우 
// 위 모두 다음과 같은 공통점을 지닌다
// "객체를 새로 생성해야 한다. 단, 생성과 동시에 동일한 자료형의 객체로 초기화 해야 한다."

/*
1st Case ---
>> int num1 = num2; // num1 이라는 이름의 메모리 공간을 할당과 동시에 num2에 저장된 값으로 초기화

2nd Case ---
>> int SimpleFunc(int n)
{
    ...
}

>> int main(void)
{
    int num = 10;
    // 호출되는 순간, 매개변수 n이 할당과 동시에 초기화 
    SimpleFunc(num);
}

3rd Case ---
int SimpleFunc(int n)
{
    // 반환되는 순간 메모리 공간이 할당되면서 동시에 초기화 
    return n;
}

반환되는 값을 별도의 변수에 저장하는 것과 별개로
값을 반환하면 반환된 값은 별도의 메모리 공간이 할당되어서
저장이 된다.

cout << SimpleFunc(1) << endl;
이 경우 출력이 된다.
반환되는 값을 메모리 공간의 어딘가에 저장해 놓지 않았다면
cout에 의한 출력이 가능할까 ?
- 출력되기 위해서는 그 값을 참조할 수 있어야 하고
- 참조가 가능하려면, 메모리 공간이 어딘가에 저장되어야 한다.

즉,
함수가 값을 반환하면, 별도의 메모리 공간이 할당되고, 
이 공간에 반환 값이 저장된다. (반환 값으로 초기화된다.)
*/

// Q. 객체 생성 및 초기화 과정 3개 예시 + 설명 

// 1st Case
Simple obj2 = obj1;

// 2nd Case --> 매개변수로 선언된 ob 객체가 생성 
// (ob 를 위한 메모리 공간이 할당된다. + 전달된 인자 obj로 초기화 == 생성과 동시에 초기화 )
Simple SimpleFunc(Simple ob)
{...}
int main()
{
    Simple obj;
    SimpleFunc(obj);
}

// 3rd Case
Simple SimpleFunc(Simple ob)
{
    return ob;
    // return 문이 실행되는 순간, Simple 객체를 위한 메모리 공간이 할당되고 
    // 이 공간에 할당된 객체는 반환되는 객체 ob의 내용으로 초기화 된다.
}

// Q. 아래의 경우, 복사 생성자의 호출 주체 ? ---
void SimpleFunc(Simple ob)
{
    ob.ShowData();
}
int main()
{
    Simple obj;
    SimpleFunc(obj);
}

// 초기화의 대상은 obj 객체가 아니라, ob 객체이다
// 그리고 ob 객체는 obj 객체로 초기화 횐다.
// 따라서, ob 객체의 복사생성자가 호출되면서, obj 객체가 인자로 전달되어야 한다.

// Q. 아래 return 문 호출의 경우, 호출과정에서의 변화 3 + 복사 생성자의 호출 주체 ? ---
class Simple
{
    Simple SimpleFunc(Simple ob)
    {
        return ob;
    }
    Simple& AddNum(int n)
    {
        return *this;
    }
}

int main()
{
    Simple obj;
    // 1. obj --> ob 의 복사 생성자 호출 ( 인자 )
    // 2. return : ob --> 임시 객체의 복사 생성자 호출 ( return 문에서, 임시객체를 생성)
    // 3. 함수 호출이 끝나면, 지역적으로 선언된 객체 ob는 사라지고 obj, 임시 객체만 남는다. 
    SimpleFunc(obj).AddNum(30);
}


// Q. return 반환시 만들어지는 임시객체는 언제 사라지는가 ??
class Temp
{
private :
    int num;
public :
    Temp(int n) : num(n){}
    void ShowInfo(){...}
}
int main()
{
    Temp(100); // 임시 객체 생성 예시

    // Q. 아래 코드가 동작하는 이유 ?
    Temp(100).ShowInfo();

    // 클래스 외부에서 객체의 멤버함수를 호출하기 위해 필요한 것은 총 가지
    // 1) 객체에 붙여진 이름
    // 2) 객체의 참조값 (객체 참조에 사용되는 정보)
    // 3) 객체의 주소값 
    // 위의 경우, 임시객체가 생성된 위치네는, 임새객체의 참조값이 반환된다
    // (임시객체의 참조값).ShowInfo();
    // 따라서, 이와 같이 이어서 멤버함수의 호출이 가능한 것이다.
    // 또한 참조값이 반환되기 때문에, 아래와 같은 표현도 가능한 것이다.
    Temp &ref = Temp(100);
}


// Q. 임시 객체의 특성 2가지
// - 다음 행으로 넘어가면 바로 소멸되어 버린다
// - 참조자에 참조되는 임시객체는 바로 소멸되지 않는다.

#include <iostream>

class A
{
public:
	A()
	{
		m_pData = new int[5]{};
	}

	A(const A& other)
	{
		m_pData = new int[5]{};
		for (int i = 0; i < 5; ++i)
		{
			m_pData[i] = other.m_pData[i];
		}
	}

	void SetDataA()
	{
		for (int i = 0; i < 5; ++i)
		{
			m_pData[i] = i + 10;
		}
	}

	void PrintDataA()
	{
		std::cout << "A" << std::endl;
		for (int i = 0; i < 5; ++i)
		{
			std::cout << m_pData[i] << std::endl;
		}
	}

	~A()
	{
		delete[] m_pData;
	}

protected:
	int* m_pData;
};

class B : public A
{
public:
	B(): m_data{5.f}
	{
	}

protected:
	float m_data;
};

class C : public B
{
public:
	C()
	{
		m_pDouble = new double[10]{};
	}

	//멤버 초기화 리스트를 제거 해보고 디버그 해보세요
	C(const C& other) : B{other}
	{
		m_pDouble = new double[10]{};
		for (int i = 0; i < 10; ++i)
		{
			m_pDouble[i] = other.m_pDouble[i];
		}
	}

	~C()
	{
		delete[]m_pDouble;
	}

	void PrintDataC()
	{
		std::cout << std::endl;
		std::cout << "C" << std::endl;
		for (int i = 0; i < 10; ++i)
		{
			std::cout << m_pData[i] << std::endl;
		}
	}

private:
	double* m_pDouble;
};

int main()
{
	C c{};
	c.SetDataA();
	C copy_c{c};
	copy_c.PrintDataA();
	return 0;
}
