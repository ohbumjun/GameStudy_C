class Simple
{
private :
    static int simpObj;
public :
    Simple(){}
}

int Simple::simpObj = 0; // static 멤버변수 초기화 

// Q. static 멤버변수의 특성 4개 .

// 1. simpObj는 Simple 객체가 생성될 때마다 함께 생성되어 , "객체 별로" 유지되는 변수가 아니다
// 2. 객체를 생성하건, 생성하지 않건, 메모리 공간에 딱 하나만 할당이 되어서 공유되는 변수이다. 
// --> Simple 클래스 안에서 선언된 static 변수는, 모든 Simple 객체가 공유하는 구조이다.
int main()
{
    Simple s1;
    Simple s2;
    Simple s3;
    // 3.
    // 각 객체의 멤버함수에서는, simObj static 멤버 변수에 접근하듯이 접근이 가능하다
    // 하지만, 그렇다고 객체 내에 simObj 가 존재하는 것은 아니다.
    // 이 변수는 객체 외부에 있다. 객체에게 멤버 변수처럼 접근할 수 있는 권한을 줬을 뿐이다.
    return 0;
}
// 4. 생성 및 소멸 시점도 전역변수와 동일하다. 

// Q. static 변수를 생성자에서 초기화하면 안되는 이유 ---
Simple()
{
    simObj = 0;
    simObj++;
};
// 위와 같이 정의하면 새로운 객체가 생성될 때마다 simObj는 0으로 초기화 된다.
// 왜냐하면, simObj는 객체가 생성될 때, 동시에 생성되는 변수가 아니라
// 이미 메모리 공간이 할당되어 있는 변수이기 때문이다.
// 따라서, 아래와 같이 정의한다.
int Simple::simObj = 0;
// 이는 Simple 클래스의 static 멤버변수 simObj가 메모리 공간에 저장될 때
// 0으로 초기화 한다는 뜻이다.


// Q. static 멤버 함수 특징 3---
// - 선언된 클래스의 모든 객체가 공유한다. 
// - public 으로 선언이 되면 , 클래스의 이름을 이용해서 호출이 가능하다
// - 객체의 멤버로 존재하는 것이 아니다.

// Q. 아래의 코드가 컴파일 에러가 나는 이유 ( 2가지 ) + 결론 
class Simple
{
private :
    int num;
    static int num2;
public :
    static void Adder(int n)
    {
        num += n;
    }
    // static 멤버함수가 멤버변수 num에 접근하는 것은 잘못이다

    // - 객체의 멤버가 아닌데, 객체의 멤버 변수에 접근할 수 없고
    // - 접근을 하더라도, 객체 외부에 존재하는 와중에, 어떤 객체의 멤버변수에 접근할지도 알 수 없다.
    // - 따라서, static 멤버함수 내에서는 static 멤버변수와 static 멤버함수만 호출이 가능하다.  
}

// Q. const static 멤버의 특징

// const 멤버변수의 경우, 멤버 이니셜라이저를 통해, 선언과 동시에 초기화가 가능했다.
// const static 변수의 초기화는 클래스내에서 선언과 동시에 초기화 할 수 있다
class Simple
{
public :
    const static int RUSSIA = 100; //
}