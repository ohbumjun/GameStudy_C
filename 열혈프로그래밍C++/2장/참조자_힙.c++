/*
포인터를 사용하지 않고도
힙에 할당된 변수에 접근하는 방법
*/

#include <iostream>

int main()
{
    int *ptr = new int;
    int &ref = *ptr;
    ref = 20;
    std::cout << ref << std::endl; // 20 출력 

    // 즉, 참조자 선언으 토애서, 포인터 연산 없이 힙 영역에 접근 가능하다. 
    return 0;
}