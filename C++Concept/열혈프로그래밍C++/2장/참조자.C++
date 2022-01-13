#include <iostream>


int main()
{
    // Q. 변수의 의미 및 참조자의 관게
    // 변수는 할당된 메모리 공간에 붙여진 이름. 그 이름을 통해서 해당 메모리 공간에 접근이 가능하다
    // 참조나는, 할당된 하나의 메모리 공간에 추가적으로 이름이 부여된 것이다. 즉, 변수의 별명

    // 2020 으로 초기화된 메모리 공간에 num1, num2 라는 이름이 붙이게 된 것이다.
    int num1  = 2020;
    int &num2 = num1; 

    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;

    // Q. 참조자의 선언가능 범위 x : 3 / o : 3
    
    // 참조자는 변수에 대해서만 선언이 가능하다 + 선언과 동시에 누군가를 참조해야 한다
    int &ref  = 20;
    int &ref1;
    int &ref2 = NULL;

    int &ref2 = num2; // 참조자를 대상으로 참조자 선언도 가능
    int &ref3 = num1; // 변수 대상
    int *ptr = &num;
    int *(&pref) = ptr; // 포인터 대상 참조 가능
    std::cout << *pref << std::endl;

    int **dptr = &ptr;
    int **(&dpref) = dptr;
    std::cout << **dpref << std::endl;

    return 0;
}
