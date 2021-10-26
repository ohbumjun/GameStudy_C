
// ��������� ��������� include �Ҷ��� "" �ȿ� �ۼ����ش�.
#include "Player.h"
#include "Monster.h"
#include "Template.h"
#include<iostream>

// ��ȯ���� : ������ϳ��� ���� include�� ���־��� ��츦 ���Ѵ�.
// ��ȯ������ �ذ��ϴ� ��� : ���漱������ �ذ�.
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
���ø���, Ÿ���� ���������� �ٲ��� �� 
����� �� �ִ� �����̴�

template<typename ���ϴ� �̸�>
*/

template<typename T>
void Output(T Number)
{
	std::cout << Number << std::endl;
	// typeid(Ÿ��) Ȥ�� typeid(Number)
	// �� �־��ָ�, �ش� Ÿ���� ���ڿ��� ���� �� �ִ�
	// name()�Լ��� ���ڿ��� ������ �ȴ�
	std::cout << typeid(T).name() << std::endl;
}

template<typename T, typename P>
void Output(T Num1, P Num2)
{
	std::cout << Num1 << std::endl;
	std::cout << Num2 << std::endl;

	std::cout << typeid(T).name() << std::endl;
	std::cout << typeid(P) << std::endl;

	// hash_code() �Լ��� �̿��ؼ�
	// ��Ÿ���� ���� hash_code�� ���� �� �ִ�.
	std::cout << typeid(T).hash_code() << std::endl;
	std::cout << typeid(P).hash_code() << std::endl;
}

/*
���ø� Ư��ȭ
- ���ø��� ����, ���� Ÿ���� ���� �� �ִ�
- �� �߿�����, Ư�� Ÿ�Կ� ����
�߰�����, ���, ������ ����� 
�ο��ϰ� ���� ��, �ٷ�
���ø� Ư��ȭ�� �����Ѵ�.
*/
template<typename T>
T Add(T num1, T num2)
{
	return num1 + num2;
}

// float Ÿ���� ���
// Add �Լ��� �� Ư���ϰ� ������ְ� �ʹ�
template<>
float Add(float num1, float num2)
{
	std::cout << "float Ÿ���Դϴ�." << std::endl;
	return num1 + num2;
}

// template<>�� �ٿ����� �ʴ� ���
// ����, template Ư��ȭ�� �ƴ϶�
// ������ Add �Լ��� ������� �Ͱ� ����
double Add(double Num1, double Num2)
{
	std::cout << "double Ÿ���Դϴ�." << std::endl;
	return Num1 + Num2;
}

int main()
{
	// CPlayer player;

	// <Ÿ��>�� ����
	// T�� �� Ÿ���� ������ �� �ִ�
	Output<int>(10);
	Output<float>(3.14f);
	// �ڵ����� type�� �ν��ϱ⵵ �Ѵ�
	Output(5.23);

	Output<int, float>(30, 5.28f);
	// �ڵ����� type�� �ν��Ѵ�
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