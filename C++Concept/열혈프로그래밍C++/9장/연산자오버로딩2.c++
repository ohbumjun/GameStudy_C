// >>, << 연산자 오버로딩 ---

// Q. cout, endl 형태 흉내내서 구현하기

namespace mystd
{
    using namespace std;

    class ostream
    {
    public :
        ostream& operator << (char * str)
        {
            printf("%s", str);
            return *this;
        }
        ostream& operator << (char ch)
        {
            printf("%c", ch);
            return *this;
        }
        ostream& operator << (int num)
        {
            printf("%d", num);
            return *this;
        }
        ostream& operator << (double d)
        {
            printf("%g", d);
            return *this;
        }
        ostream operator << (ostream& (*fp)(ostream& otm))
        {
            return fp(*this);
        }
    };

    ostream& endl(ostream &otm)
    {
        otm << "\n";
        fflush(stdout); // 입력 버퍼가 아니라, 출력 버퍼를 비운다. --> 즉, endl 은 버퍼를 지우는 작업까지 함께 진행한다.
        return otm;
    };

    ostream cout;
};

int main()
{
    using mystd::cout; // using 선언문은, 컴파일러에 mystd namespace에서 cout을 사용할 것이라고 알려준다.
    // 따라서 cout을 사용할 때마다, mystd cout을 의미한다고 가정할 것이다.
    using mystd::endl;
    
    cout << "string";
    cout << endl;
    cout << "s";
    cout << endl;
    cout << 1;
    cout << endl;
    cout << 1.1f;
    cout << endl;
}


// 대입 연산자 오버로딩 ---
// Q. 대입 연산자 4가지 특성

// - 정의하지 않으면, 디폴트 대입 연산자.가 삽입된다
// - 상속 관계에서 디폴트 대입 연산자는, 기본 클래스의 대입연산자도 호출한다. 기본 클래스의 대입연산자가 
// 정의되어 있지 않다면, 기본 클래스에서도 디폴트 대입연산자가 삽입되는 방식으로 진행한다. 
// - 디폴트 대입 연산자는, 멤버 대 멤버의 복사(얕은 복사)를 진행한다
// - 연산자 내에서, 동적 할당을 한다면, 그리고 깊은 복사가 필요하다면, 직접 정의해야 한다.

int main()
{
    Point pos1(2,3);
    Point pos2(3,4);

    pos1 = pos2; // 대입 연산자 호출 
    // pos1.operator = (pos2);
}

// Q. 아래에서 대입되는 디폴트 대입 연산자 형태  -----
class First
{
private :
    int num1, num2;
public :
    First(int n1 = 0, int n2 = 0) : num1(n1), num2(n2){}
    void ShowInfo() 
    {
        std::cout << num1 << num2 << std::endl;
    }
    First& operator = (const First& ref)
    {
        // 멤버 대 멤버 얕은 복사 진행 + 자기 자신을 return + 참조형 반환 
        num3 = ref.num1;
        num4 = ref.num2;
        return *this;
    }
};

class Second : public First
{
private :
    int num3, num4;
public :
    Second(int n3 = 0, int n4 = 0) : num3(n3), num4(n4){}
    Second& operator = (const Second& ref)
    {
        num3 = ref.num3;
        num4 = ref.num4;
        return *this;
    }
    void ShowInfo() 
    {
        std::cout << num1 << num2 << std::endl;
        std::cout << num3 << num4 << std::endl;
    }
};

int main()
{
    First first(1,2);
    First fcpy;
    fcpy = first; // 디폴트 대입 연산자가 자동으로 삽입되며, 형태는 아래와 같다
    
    // Second second(3,4);
    // Second scpy;
}

/*
First& operator = (const First& ref)
{
    // 멤버 대 멤버 얕은 복사 진행 + 자기 자신을 return + 참조형 반환 
    num3 = ref.num1;
    num4 = ref.num2;
    return *this;
}
*/

// Q. 디폴트 대입 연산자의 문제점

// --> 디폴트 복사 생성자의문제와 같다. 얕은 복사가 이뤄지므로
// 댕글링 포인터.문제가 발생할 수 있다.
// 즉, 포인터 여전히 해제된 메모리 영역을 가리킨다면, 이 포인터를 댕글링 포인터.라고 한다.

// Q. 상속 구조에서의 대입 연산자 호출 특성             -----

// 대입 연산자는 생성자가 아니다 ! 따라서 일반적인 생성자의 특징과 달리
// - 유도 클래스의 대입 연산자에서, 아무런 명시를 하지 않으면, 
// - 기초 클래스의 대입 연산자가 호출되지 않는다.

// 단, 디폴트 대입 연산자가 자동으로 삽입될 경우, 
// 부모의 대입 연산자를 호출한다.

class First
{
protected :
    int num1, num2;
public :
    First(int n1 = 0, int n2 = 0) : num1(n1), num2(n2){}
    virtual void ShowInfo() 
    {
        std::cout << num1 << num2 << std::endl;
    }
    First& operator = (const First& ref)
    {
        // 멤버 대 멤버 얕은 복사 진행 + 자기 자신을 return + 참조형 반환 
        cout << "First = operator" << endl;
        num1 = ref.num1;
        num2 = ref.num2;
        return *this;
    }
};

class Second : public First
{
private :
    int num3, num4;
public :
    Second(int n3 = 0, int n4 = 0) : num3(n3), num4(n4){}
    /*
    Second& operator = (const Second& ref)
    {
        num3 = ref.num3;
        num4 = ref.num4;
        return *this;
    }
    */
    virtual void ShowInfo() override  
    {
        std::cout << num1 << num2 << std::endl;
        std::cout << num3 << num4 << std::endl;
    }
};

int main()
{
    Second second(3,4);
    Second scpy;
    scpy = second;
    scpy.ShowInfo(); // First class의 대입 연산자 호출 
}


// 반면 아래와 같이 Second 클래스 에서 대입 연산자를 정의하고
// 기본 클래스의 대입 연산자를 호출하지 않는다면
// Frist class의 대입 연산자는 호출되지 않는다

class Second : public First
{
private :
    int num3, num4;
public :
    Second(int n3 = 0, int n4 = 0) : num3(n3), num4(n4){}
    Second& operator = (const Second& ref)
    {
        num3 = ref.num3;
        num4 = ref.num4;
        return *this;
    }
    virtual void ShowInfo() override  
    {
        std::cout << num1 << num2 << std::endl;
        std::cout << num3 << num4 << std::endl;
    }
};

// 아래와 같은 형태로 기본 클래스의 대입 연산자를 호출할 수 있다
class Second : public First
{
private :
    int num3, num4;
public :
    Second(int n3 = 0, int n4 = 0) : num3(n3), num4(n4){}
    Second& operator = (const Second& ref)
    {
        num3 = ref.num3;
        num4 = ref.num4;
        First::operator=(ref);
        return *this;
    }
    virtual void ShowInfo() override  
    {
        std::cout << num1 << num2 << std::endl;
        std::cout << num3 << num4 << std::endl;
    }
}

// 이니셜라이저와 대입 연산자 ----
// Q. 아래 호출 결과 + 이유
using namespace std;

class AAA
{
private :
    int num;
public :
    AAA(int n = 0) : num(n)
    {
        cout << "A constructor" << endl;
    }
    AAA(const AAA& refA) : num(refA.num)
    {
        cout << "A copy consructor" << endl;
    }
    AAA& operator = (const AAA& refA) 
    {
        cout << "A = operator" << endl;
        num = refA.num;
    }
};

class B
{
private :
    AAA bA;
public :
    B(const AAA& refA) : bA(refA){}
};

class C
{
private :
    AAA cA;
public :
    C(const AAA& refA) 
    {
        cA = refA;
    }
};

int main()
{
    AAA obj(12);
    cout << endl;
    B obj2(obj);
    cout << endl;
    C obj3(obj);
    return 0;
}

// 결과
A constructor

A copy consructor

A constructor
A = operator

// B의 경우 복사생성자만 호출
// C의 경우, 기본 생성자 + 대입 연산자 호출


//B 와 같이
// 멤버 이니셜라이저를 이용하면, 선언과 동시에 초기화 가 이루어지는
// 바이너리 형태의 코드가 만들어진다.
// 멤버 이니셜라이저의 경우 , 즉, B의 경우 
// AAA bA = refA;  형태의 코드가 만들어지는 것이고

// C의 경우
// 생성자의 몸체 부분에서 초기화를 진행할 경우 
// 선언과 초기화를 각각 별도의 문장에서 진행하는 형태로 바이너리 코드가 생성된다.
// AAA cA; // 선언 --> 기본 생성자
// cA = refA; // 대입 --> 대입 연산자 

// Q. 멤버 이니셜라이저를 이용한 초기화의 상대적 장점 ---
// 이러한 이유로, 멤버 이니셜라이저를 이용해서 초기화를 진행하면
// 함수의 호출 횟수를 줄일 수 있따.
// + 함수 초기화 과정을 단순화 시킬 수 있어서, 약간의 성능향상을 기대할 수 있다.
