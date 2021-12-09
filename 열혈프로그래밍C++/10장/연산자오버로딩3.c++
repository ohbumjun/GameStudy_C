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