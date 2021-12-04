/*
만약 class 혹은 struct 내부에서만 사용하는 상수 목록을 정의하고자 한다면 ?
2가지 방법
*/

// NO !
#define MAX_SPEED 200

// 만약 해당 상수들이 Car 구조체 안에서만 의미있는 상수들이라면
// 다른 영역에서 사용하도록 정의된 상수가 아니다
// 따라서, 구조체 내에 포함시키면 좋다 
struct Car
{
	enum
	{
		MAX_SPD   = 200,
		FUEL_STEP = 2,
		ACC_STEP  = 10,
		BRK_STEP  = 10
	};
};

// 만약, enum의 선언을 구조체 내부에 삽입하는 것이 부담스럽다면
// 이름 공간을 이용해서, 상수가 사용되는 영역을 명시하는 것도 좋은 방법 
namespace CAR_CONST
{
	enum
	{
		MAX_SPD = 200,
		FUEL_STEP = 2,
		ACC_STEP = 10,
		BRK_STEP = 10
	};
}

