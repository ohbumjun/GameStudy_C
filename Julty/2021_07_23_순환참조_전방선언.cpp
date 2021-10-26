
// 사용자정의 헤더파일을 include 할때는 "" 안에 작성해준다.
#include "Player.h"
#include "Monster.h"
#include "Template.h"
#include<iostream>

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
템플릿은, 타입을 가변적으로 바꿔줄 때 
사용할 수 있는 문법이다

template<typename 원하는 이름>
*/

template<typename T>
void Output(T Number)
{
	std::cout << Number << std::endl;
	// typeid(타입) 혹은 typeid(Number)
	// 를 넣어주면, 해당 타입을 문자열로 얻어올 수 있다
	// name()함수에 문자열이 들어오게 된다
	std::cout << typeid(T).name() << std::endl;
}

template<typename T, typename P>
void Output(T Num1, P Num2)
{
	std::cout << Num1 << std::endl;
	std::cout << Num2 << std::endl;

	std::cout << typeid(T).name() << std::endl;
	std::cout << typeid(P) << std::endl;

	// hash_code() 함수를 이용해서
	// 이타입의 고유 hash_code를 얻어올 수 있다.
	std::cout << typeid(T).hash_code() << std::endl;
	std::cout << typeid(P).hash_code() << std::endl;
}

/*
템플릿 특수화
- 템플릿을 통해, 여러 타입이 들어올 수 있다
- 그 중에서도, 특정 타입에 대해
추가적인, 기능, 별도의 기능을 
부여하고 싶을 때, 바로
템플릿 특수화를 적용한다.
*/
template<typename T>
T Add(T num1, T num2)
{
	return num1 + num2;
}

// float 타입일 경우
// Add 함수를 더 특별하게 만들어주고 싶다
template<>
float Add(float num1, float num2)
{
	std::cout << "float 타입입니다." << std::endl;
	return num1 + num2;
}

// template<>을 붙여주지 않는 경우
// 그저, template 특수화가 아니라
// 별도의 Add 함수를 만들어준 것과 같다
double Add(double Num1, double Num2)
{
	std::cout << "double 타입입니다." << std::endl;
	return Num1 + Num2;
}

int main()
{
	// CPlayer player;

	// <타입>을 통해
	// T에 들어갈 타입을 정해줄 수 있다
	Output<int>(10);
	Output<float>(3.14f);
	// 자동으로 type을 인식하기도 한다
	Output(5.23);

	Output<int, float>(30, 5.28f);
	// 자동으로 type을 인식한다
	Output(10, 20);

	// system("cls")
	CTemplate<int, 100> temp;
	CTemplate<float, 400> temp1;
	CTemplate<float> temp2;

	temp.TestTemplateFunction<CPlayer>();
	temp.TestTemplateFunction<CMonster>();

	std::cout << Add<int>(10, 20) << std::endl;
	std::cout << Add<float>(10.234, 3.145) << std::endl;
	std::cout << Add(3.14,3.15) << std::endl;
	return 0;
}