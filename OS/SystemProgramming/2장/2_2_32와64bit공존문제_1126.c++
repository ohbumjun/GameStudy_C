// 데이터 손실의 문제
#include<stdio.h>

// 포인터는 4byte ? 64bit 시스템에서는 8byte
int main()
{
    // arr 이라는 배열 이름은 포인터, 8byte
    int arr[10] = {0,};
    // 그런데 4byte 짜리 int 형으로 형변환 
    // 32bit 시스템에서는 문제가 발생하지 않는다
    // 포인터와 int가 4byte로 동이라
    // 하지만, 64bit 시스템에서는 문제가 발생 !
    // 왜 ? 총 8byte로 주소를 표시하는데
    // 그 중 6byte를 사용했다고 해보자
    // 즉, 2^6 메모리 만큼의 메모리에 위치해있었다묜
    // 메모리 손실이 발생할 것이다.

    // 즉, 이제는 포인터를 기본자료형으로
    // downn casting 하지 않기 !!
    int arrVal = (int)a;

    printf("pointer : %d \n", arrVal);
    return 0;
}

// Polymorphic 자료형
// 경우에 따라, 여러 역할을 맡는 것 == 다형성 
#if defined(_WIN64) // # 64 bit 기반 ㅅ러정 
    typedef __int64 LONG_PTR
    typedef unsigned __int64 ULOG_PTR;
    typedef __int64 INT_PTR
    // 64 : UINT_PTR 이 64 bit int용으로 해석
    // PTR ? 포인터가 아니다 
    typedef unsigned __int64 UINT_PTR
#else 
    typedef long LONG_PTR
    typedef unsigned long ULOG_PTR;
    typedef int INT_PTR
    // unsigned int : 32bi 용으로 해석 
    typedef unsigned int UINT_PTR
#endif

// ----
// 아래 코드는 val1, val2 변수가 저장된
// 주소값의 차이를 계산해서 출력하고자 하는 것
// 두 점 사이의 거리. 주소간의 거리를 계산하고자 하는 것 

// 문제가 있다
// UINT는 4byte
// 332bit 에서는 적절히 계산을 하지만
// 64bit에서는 적절히 계산하지 못할 수도 있다
UINT CalDistance(UINT a, UINT b)
{
    return a - b;
}

// 이를 대비해서
#if defined(_WIN64)
    typedef unsigned __int64 UINT_PTR
#else 
    typedef unsigned int UINT_PTR
#endif

// UINT_PTR 는
// 64bit 에서는 64bit로 해석
// 32bit 에서는 32bit 로 해석

// 즉, 포인터 연산에서 활용하기 위한 자료형
INT CalDistance(UINT_PTR a, UINT_PTR b)
{
    return a - b;
}

int _tmain()
{
    INT val1 = 10;
    INT val2 = 20;

    _tprintf(_T("Position : %d, %d \n"),
    (UINT)&val1, (UINT)&val2);
    _tprintf(_T("Distance : %d\n"),
    CalDistance((UINT)&val1, (UINT)&val2));

    return 0;
}

// ------------------------------------------
