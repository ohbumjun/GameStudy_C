#include<iostream>

struct Player
{
	int age;
	int height;
};
/*
구조체는 메모리 크기가 큰 구조체가
만들어질 수 있다.

(copy)그렇기 때문에 단순하게 구조체를 인자로
넘길 경우, 구조체의 모든 멤버들을
복사하게 되는 상황이 발생할 수 있다.

이 경우, 구조체의 크기가 크면 클 수록
성능이 떨어지게 된다.

해결책 : 참조(reference) 이용하기 --
변경 x일 경우, const를 붙여주기

즉, 대상의 값을 변경하지 않고
단순히 값만 얻어오려고 하는 경우 ,
참조를 이용한다.
즉, 새로운 player를 생성해서,
새로운 메모리에 할당 후,
새로운 메모리에 할당된 값을 가져오는,
복사가 일어나는 것이 아니라,
참조하여 얻어오는 것

이 경우, 인자를 넘겨주는 속도가
빠르게 향상된다.
왜냐하면, 원래 구초제 파일
그 자체가 넘어가는 것이기 때문이다.
*/

// 참조 x + const x : 바꿔도 됩니다.
void TestPlayer(Player player) {}
// 참조 o + const o : 바꾸지 마세요
void TestPlayer1(const Player& player) {
	// player.Attack = 3050;
	// 이렇게 하지마세요 ! 라는 것이다
}

/*
함수 포인터 -----------------------
- 함수 포인터 : 각 함수의 주소, 즉
메모리 상에성 어디에 위치하는가
- 포인터 변수가 어떤 int 포인터 라고 할때
int 변수의 주소를 알면, 컨트롤 할 수 있다
- 함수에도 주소가 존재한다.
따라서 해당 함수의 주소를 담아놓을
포인터 변수를 선언하고, 주소를 알고 있따면
함수포인터 변수를 이용하여
해당 함수를 호출할 수 있다
- 단, 함수 포인터는 해당 함수를 호출하는 용도로만 사용될 뿐
함수 내의 특정 지역변수에 접근 하는 등의
용도로는 사용될 수 없다.

< 함수 포인터의 형태 >
반환타입(*포인터명)(인자타입들);
의 형태로 선언된다.
*/
void OutPut()
{
	std::cout << "Output Function" << std::endl;
}
void OutPut1()
{
	std::cout << "Output1 Function" << std::endl;
}
// 아래 2개의 함수는 같은 type의 함수
// return, 인자 type이 동일하기 때문이다

int Add(int Num1, int Num2)
{
	return Num1 + Num2;
}
int Minus(int Num1, int Num2)
{
	return Num1 - Num2;
}

int main()
{
	// 함수 포인터는
	// 1) return type
	// 2) 인자 type 비교를 통해
	// 같은 타입인지를 비교할 수 있다
	void (*pFunc)();

	// 함수 포인터 변수에 함수의 주소 저장하기
	// 함수 이름 자체가, 함수의 주소이다
	pFunc = OutPut;
	// 함수 호출
	pFunc();

	pFunc = OutPut1;
	pFunc();

	int (*pFunc1)(int, int);
	pFunc1 = Add;
	std::cout << pFunc1(10, 20) << std::endl;

	pFunc1 = Minus;
	std::cout << pFunc1(20, 10) << std::endl;


	return 0;
}