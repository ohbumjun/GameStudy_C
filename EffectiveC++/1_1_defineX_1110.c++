// 1. #define을 쓰려거든 const, enum, inline

#define ASPECT_RATIO 1.653

// 1. const --------------------------------
/*
컴파일러 입장에서는 
ASPECT_RATIO가 기호식 이름으로 
전혀 보이지 않는다.

컴파일러에게 넘어가기 전에
선행 처리자(전처리기)가 밀어버리고
숫자 상수로 바꿔어 버리기 때문이다.

문제는, 숫자 상수로 대체된
코드에서 컴파일 에러라도 발생한다면
헷갈릴 수 있다.

---

< 해결법: 상수를 사용한다 >
const double AspectRatio = 1.653;


AspectRaion는 언어 차원에서 지원하는
상수 타입의 데이터이기 때문에
당연히 컴파일러의 눈에도 보이게 된다.

*/

/*
주의 사항 3가지

1. 상수 포인터 ---
- 포인터는 꼭 const 로 선언
- 포인터가 가리키는 대상까지 const
ex) const char* const authorName  = "Scott"
ex) const std::string authorName("Scott");

2. 클래스 멤버로 상수를 정의하는 경우 --- 
class Player
{
    private :
        const int score = 5;
}
#define을 멤버로 사용할 수 없다.
즉, private 형태의 #define은 사용할 수 없다는 것이다.
어떤 형태의 캡슐화 혜택도 받을 수 없다는 것이다.

반대로, 상수 데이터 멤버는 캡슐화가 된다.
*/

// 2. enum --------------------------------
/*
나열자 둔갑술 ---

class Player
{
    private :
        enum {NumTurns = 5;}
        int scores[NumTurns];
}

3_1) 동작방식은 const 보다는 #define 에 더 가깝다.
const 의 주소를 잡아내는 것은 합당하지나,
enum 주소를 취하는 것은 불법
# define 주소를 얻는 것 역시 맞지 않다.

선언한 정수 상수를 가지고 다른 사람이 주소를 얻는다든지
참조자를 쓴다는지 하는 것이 싫다면
enum이 아주 좋은 자물쇠가 될 수 있다.

*/

// 3. inline --------------------------------
/*
매크로 함수 대신 사용할 수 있는
인라인 함수

매크로 함수 같은 경우, 임의의 클래스 안에서만
사용하는 것이 불가능하지만,
인라인 함수의 경우는 가능하다 
*/

template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a : b); 
}