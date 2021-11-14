/*
컴파일러가 만들어낸 함수가 필요 없으면
확실히 이들의 사용을 금해 버리자.

복사 생성자, 기본 생성자. 소멸자.
복사 대입 연산자는, 
우리가 선언하지 않고, 외부에서
이들을 호출하려고 한다면
컴파일러가 우리 대신에 선언해버린다.

만약 복사가 절대 안되게 하고 싶다면 ?

자. 컴파일러가 생성하는 함수는
모두 공개되는, public 함수이다.

따라서, 쓰고 싶지 않은 함수는
private으로 세팅하면 된다.

하지만, 여전히 해당 클래스의 멤버함수
및 프렌드 함수가 호출할 수 있다.

이를 위해, 
아예 정의를 안해버리면 안될까 ?
*/

class HomeForSale
{
    public : 
    // 
    private :
        HomeForSale(const HomeForSale); // 선언 only, 정의 X
        HomeForSale& operator = (const HomeForSale);
}

// 링크 시점 에러를, 컴파일 시점 에러로 옮겨 보자
class Uncopyable
{
    protected : // 파생된 객체에 대해
        Uncopyable(); // 생성, 소멸 허용
        ~Uncopyable();
    private :
        // 복사는 방지 한다
        Uncopyable(const Uncopyable);
        Uncopyable operator = (const Uncopyable)&;
}

class HomeForSale : private Uncopyable
{}

// HomeForSale의 객체의 복사를
// 외부에서 시도하려고 할때
// ex) 멤버 함수, 프렌드 함수
// 컴파일러는 HomeForSale 클래스만의
// 복사 생성자와 복사 대입 연산자를 만드려고 할 것이다

// 컴파일러가 생성한 복사 함수는
// 기본 클래스의 대응 버전을 호출하게 되어 있다
// 그런데, 이러한 호출은 이제 안통한다
// 복사 함수들이 기본 클래스에서 공개되어 
// 있지 않기 때문이다.