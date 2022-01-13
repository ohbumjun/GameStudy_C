#include<iostream>

int my_num(int n){return n;};
int my_num(int a, int b){return a + b;}
// void my_num(int n);

int main()
{
    my_num(1);
    my_num(1,2);
    return 0;
}

// !1. my_num 동일 이름의 함수의 정의가 허용되는 이유 ?

// - 함수 호출시, 전달되는 인자를 통해서 호출하고자 하는
// - 함수의 구분이 가능해지기 때문에, 
// - 매개변수의 선언형태( 매개 변수 개수 혹은 매개변수 type ) 가 다르다면, 동일한 이름의 함수 정의가 허용된다. 

// Q2. void my_num(int n); 는 허용되지 않는 이유
// 반환형이 다르다. 
// 반환형은, 함수 호출시, 호출되는 함수를 구분하는 기준이 될 수 없다. 
