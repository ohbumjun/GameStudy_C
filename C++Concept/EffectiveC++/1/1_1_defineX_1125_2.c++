// 1. #define을 쓰려거든 const, enum, inline

#define ASPECT_RATIO 1.653


// 1. const -----------------------------------------------------------------------------------------------------------------------------
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
*/
const double AspectRatio = 1.653;

/*
< 해결법: 상수를 사용한다 >

AspectRaion는 언어 차원에서 지원하는
상수 타입의 데이터이기 때문에
당연히 컴파일러의 눈에도 보이게 된다.

*/

/*
#define을 상수로 교체하려 할 때, 주의 사항 2가지 ------------------------

1. 상수 포인터 ---
- 포인터는 꼭 const 로 선언
- 포인터가 가리키는 대상까지 const 를 해야 한다 
*/
ex) const char* const authorName  = "Scott"
ex) const std::string authorName("Scott");

/*
2. 클래스 멤버로 상수를 정의하는 경우, 즉, 클래스 상수를 정의하는 경우 ----
- 어떤 상수의 유효범위를 클래스로 한정하고자 할때는
그 상수를 멤버로 만들어야 한다.
- 상수 사본 개수가 한개를 넘지 못하게 하고 싶다면
정적 멤버로 만들어야 한다.
*/

// Case 1 -----------
class Player{
    private :
        static const int NumTurns = 5; // 상수 선언
        int scores[NumTurns];          // 상수를 선언하는 부분 

        /*
        주의할 것은, NumTurns는 선언, Declaration 이지, "정의"가 아니다

        사용하고자 하는 것에 대해 "정의"가 마련되어야 보통이지만
        정적 멤버로 만들어지는 정수류 char, bool ,각종 정수 타입 --> 클래스 내부 상수는 예외

        이들에 대해 주소를 취하지 않는 한, 정의없이 선언만 해도 문제 없다. 

        단, 클래스 상수의 주소를 구한다든지, 
        컴파일러가 정의를 요구하는 경우에는

        const int GamePlayer::NumTruns; 
        이렇게 정의를 해야하고,
        클래스 상수의 정의는 구현 파일에 둔다.

        헤더 파일에 두지 않는 이유는
        정의에는 상수의 초기값이 있으면 안되는데
        왜냐하면 클래스 상수의 초기값은 해당 상수가 선언된 시점에 바로 주어지기 때문입니다
        즉, NumTurns 는 선언될 당시에 바로 초기화 되는 것 
        */
}

// case 2 ----------- 만일 컴파일러가 먹지 않는다면 
class Player{
    private :
        static int cost;
}

// 구현 파일
const int Player::cost = 5; 


// case 3 : 나열자 둔갑술  ------------------------------------------------------------------------- 예외 : 해당 클래스를 컴파일하는 도중에 클래스 상수의 값이 필요할 때
class Player
{
    private :
        enum {NumTurns = 5;}
        int socres[NumTurns];
}

/*
3_1) 동작방식은 const 보다는 #define 에 더 가깝다.
const 의 주소를 잡아내는 것은 합당하지나,
enum 주소를 취하는 것은 불법
# define 주소를 얻는 것 역시 맞지 않다.

선언한 정수 상수를 가지고 다른 사람이 주소를 얻는다든지
참조자를 쓴다는지 하는 것이 싫다면
enum이 아주 좋은 자물쇠가 될 수 있다.

제대로 만들어진 컴파일러는 정수 타입의 const 객체에 대해
저장공간을 저장하지 않겠지만, 
불편하게 만들어진 컴파일러는, 반대로 동작할 수 있다.
const 객체에 대한 메모리를 만들지 않는 방법을 사용하고 싶다면
위 방법이 좋은 코드이다. 
*/




// 3. inline -----------------------------------------------------------------------------------------------------------------------------
/*
매크로 함수 대신 사용할 수 있는
인라인 함수

매크로 함수 같은 경우, 임의의 클래스 안에서만
사용하는 것이 불가능하지만,
인라인 함수의 경우는 가능하다 
*/

// 매크로 함수
#define CALL_WITH_MAX(a,b) f((a) > (b) ? (a) : (b))

template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a : b); 
}

// 인라인 함수의 경우
// 임의의 클래스 안에서도 사용할 수 있다
template<typename T>
inline void callWithMax(const T&, const T& b)
{
    f(a > b ? a : b );
}