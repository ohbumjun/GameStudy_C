
// 사용자정의 헤더파일을 include 할때는 "" 안에 작성해준다.
#include "Player.h"
#include "Monster.h"
#include "Template.h"

// 순환참조 : 헤더파일끼리 서로 include를 해주었을 경우를 말한다.
// 순환참조를 해결하는 방법 : 전방선언으로 해결.
//
//void OutputNumber(int Number)
//{
//    std::cout << Number << std::endl;
//}
//
//void OutputNumber(float Number)
//{
//    std::cout << Number << std::endl;
//}

/*
템플릿은 타입을 가변적으로 바꿔줄 때 사용할 수 있는 문법이다.
template <typename 원하는이름>
template <class 원하는이름> 이런 식으로 사용한다.
template <typename T, typename T1> 이런식으로 몇개든 타입을 가변적으로 지정할 수 있다.
*/
template <typename T>
void Output(T Number)
{
    std::cout << Number << std::endl;

    // typeid(타입) 을 넣어주면 해당 타입을 문자열로 얻어올 수 있다.
    // typeid(변수) 을 넣어주면 해당 타입을 문자열로 얻어올 수 있다.
    // name() 함수에 문자열이 들어오게 된다.
    std::cout << typeid(T).name() << std::endl;
    //std::cout << typeid(Number).name() << std::endl;
}

template <typename T, typename T1>
void Output(T Number, T1 Number1)
{
    std::cout << Number << std::endl;
    std::cout << Number1 << std::endl;

    std::cout << typeid(T).name() << std::endl;
    std::cout << typeid(T1).name() << std::endl;

    // hash_code() 함수를 이용해서 이 타입의 고유한 번호를 얻어올 수 있다.
    std::cout << typeid(T).hash_code() << std::endl;
    std::cout << typeid(T1).hash_code() << std::endl;
}

/*
템플릿 특수화 : 템플릿은 다양한 타입으로
변환되어 들어올 수 있다.
다양한 타입중 내가 원하는 특정 타입만 함수 자체를
새로 구현할 수 있는 기능을 제공하는데 그것을 템플릿 특수화
라고 한다.
*/
template <typename T>
T Add(T Number1, T Number2)
{
    return Number1 + Number2;
}

// float 타입일 경우 Add함수를 좀 더 특별하게 구성해보고자 한다.
template <>
float Add(float Number1, float Number2)
{
    std::cout << "float 타입입니다." << std::endl;
    return Number1 + Number2;
}

double Add(double Number1, double Number2)
{
    std::cout << "double 타입입니다." << std::endl;
    return Number1 + Number2;
}

int main()
{
    CPlayer player;

    player.Output();

    CMonster    monster;

    // <타입> 을 통해 T에 들어갈 타입을 정해줄 수 있다.
    Output<int>(10);
    Output<float>(3.14f);

    // 아래처럼 들어간 인자의 타입으로 자동으로 타입이 정해질 수도 있다.
    Output(5.23);

    Output<int, float>(30, 5.2832f);
    Output(10, 20);

    system("cls");

    CTemplate<int, 100>   temp;
    CTemplate<float, 400>   temp1;
    CTemplate<float>   temp2;


    temp.TestTemplateFunction<CPlayer>();
    temp.TestTemplateFunction<CPlayer>();
    temp.TestTemplateFunction<CPlayer>();
    temp.TestTemplateFunction<CPlayer>();
    temp.TestTemplateFunction<CPlayer>();
    temp.TestTemplateFunction<CMonster>();

    std::cout << Add<int>(10, 20) << std::endl;
    std::cout << Add<float>(10.235, 3.14f) << std::endl;
    std::cout << Add<double>(10.235, 3.14) << std::endl;
    std::cout << Add(3.14, 223.344) << std::endl;

    // 아래는 #deinfe COUNT 10 으로 되어있는 define을 사용했다.
    // 이 경우 컴파일 하기 전에 Visual Studio는 10을 여기에 붙여서
    // std::cout << 10 << std::endl; 이렇게 코드를 만들어놓고
    // 컴파일을 하게 된다.
    std::cout << COUNT << std::endl;

    // std::cout << "PrintDefine" << std::endl;
    PRINTDEFINE;


    return 0;
}
