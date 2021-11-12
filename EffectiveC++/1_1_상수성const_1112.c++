
char greeting[] = "Hello";

// 비상수 포인터 ----------
// 상수 데이터
const char* p = greeting;


// 상수 포인터
// 상수 데이터 
const char* const p = greeting;

// 상수 데이터 2가지 방식
void f1(const Widget* pw);
void f2(Widget const *pw);

// STL -------------
// 어떤 반복자를 const로 선언하는 일은
// 포인터를 상수로 선언하는 것
const std::vector<int>::iterator iter = vec.begin();

*iter = 10; // ok
++iter // 불가능 

// 변경이 불가능한 객체를 가리키는 반복자가 필요 ?
// const_iterator
std::vector<int>::const_iterator cIter = vec.begin();

*cIter = 10; // 불가능
++cIter; // 가능 

// 상수 멤버 함수
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
// 만약 operaotr[]가 그냥 char를 반환하면
// 아래는 컴파일 에러가 난다
tb[0] = 'x';

// 왜냐하면, 기본제공 타입을 반환하는 함수는
// 반환 값을 수정하는 일이 절대로 
// 있을 수 없기 때문이다
// 즉, 위의 경우, 수정되는 값은 tb.text[0]의 사본이지
// tb.text[0] 자체가 아니기 때문이다.

// 상수성의 3가지 종류 ----
