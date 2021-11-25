
char greeting[] = "Hello";

// 상수 데이터, 상수 포인터 -------------------------------------------------------------
// 비상수 포인터
// 상수 데이터
const char* p = greeting;


// 상수 포인터
// 상수 데이터 
const char* const p = greeting;

// 상수 데이터 2가지 방식
void f1(const Widget* pw);
void f2(Widget const *pw);

// const + iterator -------------------------------------------------------------------
// 어떤 반복자를 const로 선언하는 일은
// 포인터를 상수로 선언하는 것
const std::vector<int>::iterator iter = vec.begin();
*iter = 10; // ok
++iter // 불가능 

// 변경이 불가능한 객체를 가리키는 반복자
// 즉, 포인터 자체는 변경 가능, 다만, 가리키는 대상은 변경 불가 == const iterator
std::vector<int>::const_iterator cIter = vec.begin();

*cIter = 10; // 불가능
++cIter; // 가능 

// 상수 멤버 함수 ---------------------------------------------------------------------
// 멤버 함수에 붙는 const 키워드의 역할은
// 해당 멤버함수가 상수 객체에 대해
// 호출될 함수이다.

// 1) 클래스의 인터페이스를 이해하기 좋다
// 그 클래스로 만들어진 객체를 변경할 수 있는 함수는 무엇이고
// 또 변경할 수 없는 함수는 무엇인가

// 2) 상수 객체를 사용할 수 있게 하자
// 객체전달은 보통 "상수 객체에 대한 참조자"로 진행한다
// 이 기법이 제대로 살아 움직이려면
// 상수 상태로 전달된 객체를 조작할 수 있는
// const 멤버 함수, 즉, 상수 멤버함수가
// 준비되어 있어야함 한다.

// 참고 : const 가 있고 없고의 차이만 있는 멤버 함수들은
// 오버로딩이 가능하다

class TextBlock
{
    public :
        // 상수 객체에 대한 operator 
        const char& operator[] (~) const {return ~;}
        // 비상수 객체에 대한 opearator 
        char& operator [] {return ~;}
}

class TextBlock
{
public :
// 상수 객체에 대한 operator[]
    const char& operator[] (~) const {return ~}
// 비상수 객체에 대한 operator[]
    char& operator[] () {return ~}
} 

TextBlock tb("Hello");
cout << tb[0] // 비상수 멤버 호출

const TextBlock ctb("world");
cout << ctb[0]; // 상수 멤버 호출 

// 아래가 컴파일 에러가 나는 이유 ? ---
ctb[0] = 'x'

// operator[] 연산자 자체에는 문제가 없다
// return type 때문이다
// 즉, const char& 타입에 대한
// 연산을 시도했기 때문이다


// 비상수 멤버의 char 참조자(reference) 반환 ---
// 만약 operaotr[]가 그냥 char (char& 이 아니라) 를 반환하면
// 아래는 컴파일 에러가 난다
tb[0] = 'x';

// 왜냐하면, 기본제공 타입을 반환하는 함수는
// 반환 값을 수정하는 일이 절대로 
// 있을 수 없기 때문이다
// 즉, 위의 경우, 수정되는 값은 tb.text[0]의 사본이지
// tb.text[0] 자체가 아니기 때문이다.

// 상수성의 2가지 종류 ----
// 1. 비트 수준 상수성 == 물리적 상수성
// 2. 논리적 상수성

/*
비트 수준 상수성은, 어떤 멤버함수가
그 객체의 어떤 데이터 멤버도 건드리지 않아야 합니다.
(단, 정적 멤버 제외)

그 멤버함수가 const 임을 인정하는 개념
즉, 객체를 구성하는 비트들 중 어떤 것도
바꾸면 안된다는 것이다.

주목할 점은, 제대로 const로 동작하지 않는데도
이 비트 수준 상수성 검사를 통과하는 멤버함수들이
적지 않다는 점이다.
어떤 포인터가 가리키는 대상을 수정하는 멤버함수들 중
상당수가 그렇다.

아래의 경우, operator[] 함수가 상수 멤버함수로 선언되어 있다.
멤버함수 그 어떤 것도 건드리지 않기 때문에
비트 수준 상수성은 통과한다.
*/
class CTextBlock{
    public :
        char& operator [] (std::size_t position) const 
        {
            return pText[position];
        }
    private :
        char *pText;
}

const CTextBlock cctb("Hello");
// 내부 데이터에 대한 포인터를 얻는다.
char*pc = &cctb[0];
// 값을 수정할 수 있게 된다.
*pc = 'J';

// 잘못된 것이다.
// 어떤 값으로 초기화된 상수 객체를 하나 만들어두고
// 이것에다 상수 멤버를 호출했더니
// 값이 변해버린 것이다. 

/*
논리적 상수성이라느 개념.
상수 멤버 함수라고 해서, 객체의 한 비트도 수정할 수 없는 것이 아니라
일부 몇 비트 정도는 바꿀 수 있되,
그것을 사용자 측에서 알아채지 못하게만 하면
상수 멤버 자격이 있다는 것이다 
*/

class CTextBlock
{
public :
    std::size_t length() const ;
private :
    char* pText;
    std::size_t textLength; // 바로 직전에 계산한 텍스트 길이
    bool lengthIsValid; // 이 길이가 유요한다
}

// 아래의 코드는 비트 수준 상수성이랑 매우 멀다
// textLength, lengthIsValid 멤버 변수가 변할 수 있기 때문이다
std::size_t CTextBlock::length() const 
{
    if(!lengthIsValid)
    {
        textLength = std::strlen(pText);
        lengthIsValid = true;
    }
    return textLength;
}

// Mutable 사용하기 -------------------------------------------------------------------------
class CTextBlock
{
public :
    std::size_t length() const ;
private :
    char* pText;
    mutable std::size_t textLength; // 해당 데이터 멤버들은 어떤 순간에도 수정이 가능합니다. 상수 멤버 함수안에서도 수정할 수 있습니다 
    mutable bool lengthIsValid; 
}

// 상수멤버 및 비상수멤버 함수에서 코드 중복 현상을 피하는 방법 ----------------------------
// 만약 특정함수의 상수 버번이, 비상수버전과 하는 일이 똑같다면 ?
// 유일한 차이가 const 가 있냐 없냐의 차이라면 ?
// 이는 코드의 중복이다 

// 이러한 코드의 중복을 없애기 위해서는
// 비상수 operator[]가 상수 버전을 호출하도록 구현하는 것이다 

class TextBlock
{
    public :
        const char& operator[] (std::size_t position) const 
        {
            return text[position];
        }
        // 지금 하는 일은 비상수 operator[] 가 
        char& operator[](std::size_t position)
        {
            return 
                // operator[] 반환 값에서
                // const를 떼어내는 캐스팅
                // const 를 제거하는 캐스팅은
                // const_cast 밖에 없다.
                const_char<char&>(
                    // 2번째 ---
                    // 원래의 타입인 TextBlock& 에서
                    // const TextBlock& 으로 바꾸는 것이다
                    // *this에 const 를 붙이는 것
                    // const operator [] 을 호출하겠다 !
                    static_cast<const TextBlock>(*this)[position];
                )
        }
}