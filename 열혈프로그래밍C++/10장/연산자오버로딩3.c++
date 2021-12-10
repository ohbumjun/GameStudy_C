// Q. const 기반 오버로딩 ? ---

// const 선언 유무도 함수 오버로딩 조건에 해당한다.

class A
{
public :
    void show() 
    {
        cout << "a" << endl;
    }
    void show() const  // 이것도 함수오버로딩의 한 형태.
    {
        cout << "a const" << endl;
    }
};

int main()
{
    A a;
    const A c_a;
    
    a.show();    // "a"
    c_a.show();; // "a const"
}

// new 연산자 오버로딩 ---

// Q. new 연산자가 하는 일 3가지 >

// - 메모리 공간의 할당
// - 생성자의 호출
// - 할당하고자 하는 자료형에 맞게 반환된 주소값의 형변환
// ( C언어에서 사용하던 malloc 함수는 자료형 변환 과정을 앞에 붙여줘야 했으나 new는 필요없다. )

// Q. new 연산자가 오버로딩 하난 part >

// - 메모기 공간의 할당 --> 이 부분만 오버로딩 할수 있도록 컴파일러가 만들어짐

// Q. new 연산자 오버로딩 형태 + 예시

// void * operator new (size_t size) {...}
// - 반환형은 void * 여야 한다
// - 매개변수형은 size_t 여야 한다.

// ex) Point* ptr = new Point(3, 4);
// >> 먼저 필요한 메모리 공간을 계산한다 --> 바이트 단위 계산  
// >> 그 크기가 계산되면 , operator new 함수를 호출하면서 계산된 크기의 값을 인자로 전달한다.

/*

void * operator new (size_t size)
{
    void * adr = new char[size]; // --> 필요한 메모리 공간의 크기가, 바이트 단위로 계산되어서 인자로 전달된다. 
                                        따라서, 크기가 1 바이트인 char 단위로 메모리 공간을 할당해서 반환하였다.

                                        이렇게 operator new 함수가 할당한 메모리 공간의 주소값을 반환하면
                                        컴파일러는 생성자를 호출해서 , 메모리 공간을 대상으로 초기화를 진행한다.

                                        생성자의 호출 정보는, operator new 함수와 아무런 관련이 없다.
                                        생성자의 호출은, 여전히 컴파일러의 몫
                                        operator new 함수의 반환 정보가, 컴파일러에 의해 참조될 뿐이다. 
    return adr;                  
}


마지막으로 
Point* ptr = new Point(3, 4);
- 이 문장에서 new 연산자가 반환하는 값은, operator new 함수가 반환하는 값이 아니다
할당하고자 하는 자료형에 맞게 컴파일러에 의해 형변환된 값이다.



*/

// Q. delete 연산자 오버로딩 형태 및 소멸자와의 관계

Point* ptr = new Point(3,4);
delete ptr;
void operator delete (void * adr)
{
    delete [] adr;
}

// 컴파일러는 delete ptr을 통해 객체의 소멸자를 호출
// 소멸자가 호출된 "다음" 위와 같읕 형태의 함수에 ptr 에 저장된 주소값을 전달
// 즉, 해당 오버로딩된 함수가, 전달된 주소값을 기반으로 메모리 공간의 소멸을 책임져야 한다. 


// Q. new 연산자의 의아한 점

// - new 연산자가 객체의 멤버함수 형태로 오버로딩이 되었다면,
// 객체가 생성된 이후에 호출이 되어야 하는데, new 연산자는 객체가 생성되기 전, 객체를 생성할 때 호출하는 연산자....!!!??
// new 연산자가, static 함수이기 때문에, 객체 생성 과정에서 호출이 가능했던 것이다. 


// Q. operator new [], operator delete [] 형태 및 기능 

// void * operator [] (size_t size){...} : new 연산자를 이용한 배열할당시 호출되는 함수
// void operator delete [] (void* adr) : 배열소멸시 호출되는 함수 


// Q. 포인터 연산자 오버로딩 ------------------------------------------------------------------------------------

// Q. 대표적인 포인터 연산자 2가지

// - '->' : 포인터가 가리키는 객체의 멤버에 접근
// - '*'  : 포인터가 가리키는 객체에 접근 

// Q. 문제
using namespace std;

class Number
{
private :
	int num;
public :
	Number(int n) : num(n){}
	void ShowData() 
	{
		cout << num << endl;
	};
	Number* operator -> ()
	{
		return this;	
	};	
	Number& operator * ()
	{
		return *this;
	};
};

int main()
{
	Number num(20);
	num.ShowData();
	
	(*num) = 30;        // Q1. 여기서 발생하는 현상 
	num->ShowData();    // Q2. 여기서 이루어지는 함수 호출의 형태 /
	
	(*num).ShowData();
	return 0;
}

// Q1. 답변 
// 아래와 같은 대입 연산자가 위의 형태로 이루어진다
// (*num)을 통해서, 객체자신을 참조형의 형태로 반환하고 있다. * 연산자 오버로딩의 결과  
// 실제 (num.operator*()) = 30 의 형태 
Number& operator = (int p_num)
{
    cout << "= operator" << endl;
    num = p_num;
};

// Q2. 답변
// num.operator -> () ShowData();
// - num.operator -> () 가 반환하는 것은 그런데 주소값이다. 따라서 여기서 바로 ShowData 함수 호출은
// 문법적으로 성립하지 않는다. 
// 따라서, 반환되는 주소값을 대상으로, 적절한 연사니 가능하도록 -> 연산자를 한번 더 추가하여 해석을 진행한다.

// num.oprator -> () -> ShowData();

// >> () 연산자 오버로딩과 Functor 

// Q. adder(2,4) 의 연산 형태 ?

// 함수 호출에 사용되는, 인자의 전달에 사용되는  () 도 연산자.이다.
// 이 연산자를 오버로딩 하면, 객체를 함수처럼 사용할 수도 있다
// adder.operator () (2,4);

// Q. Functor 의 의미 및 예시

// 함수처럼 동작하는 클래스를 가치켜 펑터(Functor) , 혹은 "함수 오브젝트 Function Object" 라고도 불린다.
class Adder
{
public :
    int operator () (int n1, int n2)
    {
        return n1 + n2;
    }
}

int main()
{
    Adder add;
    cout << add(2,3) << endl;
}

// >> 임시 객체로의 자동 형변환 + 형 변환 연산자 (Conversion Operator) ---

class Number
{
private :
    int num;
public :
    Number (int n = 0) : num(n)
    {
        cout << "Number constructor" << endl;
    }
    Number& operator = (const Number& ref)
    {
        cout << "Number operator =" << endl;
        num = ref.num;
        return *this;
    }
    void ShowNumber()
    {
        cout << "num : " << num << endl;
    }
};
int main()
{
    Number num(20);
    Number num2(30);

    num = num2; // 동일한 자료형의 객체간 대입 연산
};

// Q. 위와 같이 동일한 자료형의 객체간 대입 연산이 아니라, 다른 자료형의 객체가 대입 연산이 이루어지는 원리 ---

int main()
{
    Number num(20);
    
    num = 30;
};

// A 형 "객체"가 와야할 위치에 B형 "데이터" 혹은 B형 "객체"가 왔을 경우
// B형 데이터를 인자로 전달받는 A형 클래스의 생성자 호출을 통해서, A형 임시 객체를 생성한다.

num = 30; ?
// 1) num = Num(30);
// 2) num.operator = (Number(30))
// 이러한 2단계의 대입연산이 진행된다는 의미이다.

// 즉, 기본 자료형 데이터를. 객체로 형 변환하는 것은, 적절한 생성자의 정의를 통해 얼마든지 가능하다.

// Q. 반대로, 객체를 기본 자료형으로 형변환하는 방법 (아래 연산이 이루어지는 2가지 방법)  ---
int main()
{
    Number num1(20);
    
    Number num2 = num1 + 30;
};

// 1. Number 클래스가 + 연산자 오버로딩을 수행하거나
// 2. Number 클래스가 int 형 데이터로 형변환되거나 
class Number
{
public :
    operator int () // 형변환 연산자 오버로딩
    {
        return num;
    }
};

// 형변환 연산자는, 반환형을 명시하지 않는다
// 하지만, return 에 의한 값 반환은, 얼마든지 가능하다
// 위의 함수는 int 형으로 형변환 할때 호출되는 함수! 라고 생각하면 된다.