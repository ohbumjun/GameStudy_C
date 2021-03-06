/*
비트단위 논리 연산자

진수
- 2, 8, 10, 16 진수

2진수
- 0 ~ 1 까지 

8진수
- 0 ~ 7

10 진수
- 0 ~9 

16 진수
- 0 ~ 15
--   0 ~ 9 
-- 10 ~ 15 : a ~ f

진수들은, 서로 다른 진수로 변환이 가능하다

---
87을 2진수, 16진수로 변환한다.
87을 2진수로 변환한다 ?
- 계속 2로 나눠가면 된다
87 / 2 = 43 -- 1
43 / 2 = 21 -- 1
21 / 2 = 10 -- 1
10 / 2 = 5  -- 0
5   / 2 = 2  -- 1
2   / 2 = 1 -- 0

1010111

16진수로 변환
= 2진수로 변환, 그 다음에 16진수로 변환 시키기 

1010111 을 오른쪽에서부터 4자리씩 끊어서 계산한다.
16진수 1자리는 2진수 4자리이다.

0101 , 0111
8421   8421

2진수가 1인 부분의 값들을 더한다
	5    7

따라서 16진수로는 57 이 된다.

---

비트 단위 논리연산자란,
비트 단위, 즉, 2진수 단위, 연산을 하는 연산자.이다.

값대 값을 연산하여 값으로 나온다.
종류 : AND (&), OR(|), NOT(~), XOR(^ = 서로 다르면 1, 같으면 0)

87 & 53 ?

53 = 0110101
87 = 1010111 &
     = 0010101 
	 = 1 + 4 + 16 = 21

---
>> 활용 예시
- 버프를 저장
- 각각의 비트 단위로, 어떤 것은 공격, 방어, mp 버프 등
- int 는 4byte는 32bit, 
- 변수 하나로, 32개의 상태를 저장할 수 있다는 것  


*/

/*

#include<iostream>

int main()
{
	std::cout << (87 & 53) << std::endl; // 21

	// 16진수 1자리 == 2진수 4자리 == 4bit
	// 16진수 8자리 == 32bit 

	const int iAttack = 0x00000001; // 1
	const int iArmor = 0x00000002; // 10
	const int iHP      = 0x00000004; // 100
	const int iMP      = 0x00000008; // 1000
	const int iCritical = 0x00000010; // 10000

	// 001 | 100        = 101
	// 00101 | 10000 = 10101
	// 즉, buf 들이 있는 부분만 1 --> buf 추가할 때에는 or연산을 통해 추가한다.
	int iBuf = iAttack | iHP | iCritical;

	// 10101 & 00001 = 1
	std::cout << "Attck : " << (iBuf & iAttack) << std::endl;   // Attack이 존재한다. 
	// 10101 & 00001 = 1
	std::cout << "Armor : " << (iBuf & iArmor) << std::endl; // Armor가 없다.

	// And 연산을 통해서는, 각 비트가 켜져있는지 꺼져있는지 알 수 있다
	
	// ----
	// 연산자 축약형
	iBuf ^= iHP;

	// 쉬프트 연산자 : << , >> = 값 대 값을 연산하여 값으로 나오게 된다.
	// 20 << 2 
	// 1) 20을 2진수로 변환한다
	// 10100
	// 1010000 : 왼쪽 2칸 이동 == 80 == 20 * 4
	// 즉, 비트 하나가 왼쪽으로 갈때마다 * 2 ( 반대로, 오른쪽으로 하나 갈때마다 / 2 )
	
	// 20 >> 3 = 20 / 8 == 2
	// 10100 --> 10 
	
	// 즉, 2^n 승 단위의 나눗셈을 할때는, 쉬프트 논리 연산자를 활용하면 좋다.
	// ---
	// 비트 쪼개서 사용하기
	const int iHigh = 187;
	const int iLow  = 13560;

	int iNumber = iHigh; // 
	iNumber <<= 16;    // 상위 16비트를 채운다

	iNumber |= iLow; // or 연산 -->추가 --> 하위 16비트를 채운다

	std::cout << "High : " << (iNumber >> 16) << std::endl;
	std::cout << "Low  : " << (iNumber & 0x0000ffff) << std::endl; // 하위 16비트를 모두 1 로 채운다 


	return 0;
}

*/