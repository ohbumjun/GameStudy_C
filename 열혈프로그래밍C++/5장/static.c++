class Simple
{
private :
    static int simpObj;
public :
    Simple(){}
}

int Simple::simpObj = 0; // static 멤버변수 초기화 

// Q. static 멤버변수의 특성

// 1. simpObj는 Simple 객체가 생성될 때마다 함께 생성되어 , "객체 별로" 유지되는 변수가 아니다
// 2. 객체를 생성하건, 생성하지 않건, 메모리 공간에 딱 하나만 할당이 되어서 공유되는 변수이다. 
// 