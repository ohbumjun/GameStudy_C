// 데이터 손실의 문제

#include<stdio.h>

// 포인터는 4byte, 64bit 시스템에서는 8byte 
int main()
{
	// arr 이라는 배열 이름은 포인터 이기도 하다. 8byte
	int arr[10] = { 10, };

	// 그런데 4byte 짜리 int 형으로 형변환
	// 32bit 시스템에서는 문제가 발생하지 않느나.
	// 포인터와 int가 4byte로 동일하기 때문ㅇ데
	// 하지만 64bit 에서는 문제가 된다.
	// 왜냐하면 총 8byte 로 주소를 표시하는 데
	// 그 중 6byte를 사용했다고 해보자
	// 그러면 이를 4byte 단위로 형변환 하게 되면, 메모리 손실이 발생할 것이다

	// 즉,이제는 포인터를 기본자료형으로 down casting 하지 않기
	int arrVal = (int)arr;

	return 0;
}

// 