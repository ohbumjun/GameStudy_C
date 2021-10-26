
// ��������� ��������� include �Ҷ��� "" �ȿ� �ۼ����ش�.
#include "Player.h"
#include "Monster.h"
#include "Template.h"

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
���ø��� Ÿ���� ���������� �ٲ��� �� ����� �� �ִ� �����̴�.
template <typename ���ϴ��̸�>
template <class ���ϴ��̸�> �̷� ������ ����Ѵ�.
template <typename T, typename T1> �̷������� ��� Ÿ���� ���������� ������ �� �ִ�.
*/
template <typename T>
void Output(T Number)
{
    std::cout << Number << std::endl;

    // typeid(Ÿ��) �� �־��ָ� �ش� Ÿ���� ���ڿ��� ���� �� �ִ�.
    // typeid(����) �� �־��ָ� �ش� Ÿ���� ���ڿ��� ���� �� �ִ�.
    // name() �Լ��� ���ڿ��� ������ �ȴ�.
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

    // hash_code() �Լ��� �̿��ؼ� �� Ÿ���� ������ ��ȣ�� ���� �� �ִ�.
    std::cout << typeid(T).hash_code() << std::endl;
    std::cout << typeid(T1).hash_code() << std::endl;
}

/*
���ø� Ư��ȭ : ���ø��� �پ��� Ÿ������
��ȯ�Ǿ� ���� �� �ִ�.
�پ��� Ÿ���� ���� ���ϴ� Ư�� Ÿ�Ը� �Լ� ��ü��
���� ������ �� �ִ� ����� �����ϴµ� �װ��� ���ø� Ư��ȭ
��� �Ѵ�.
*/
template <typename T>
T Add(T Number1, T Number2)
{
    return Number1 + Number2;
}

// float Ÿ���� ��� Add�Լ��� �� �� Ư���ϰ� �����غ����� �Ѵ�.
template <>
float Add(float Number1, float Number2)
{
    std::cout << "float Ÿ���Դϴ�." << std::endl;
    return Number1 + Number2;
}

double Add(double Number1, double Number2)
{
    std::cout << "double Ÿ���Դϴ�." << std::endl;
    return Number1 + Number2;
}

int main()
{
    CPlayer player;

    player.Output();

    CMonster    monster;

    // <Ÿ��> �� ���� T�� �� Ÿ���� ������ �� �ִ�.
    Output<int>(10);
    Output<float>(3.14f);

    // �Ʒ�ó�� �� ������ Ÿ������ �ڵ����� Ÿ���� ������ ���� �ִ�.
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

    // �Ʒ��� #deinfe COUNT 10 ���� �Ǿ��ִ� define�� ����ߴ�.
    // �� ��� ������ �ϱ� ���� Visual Studio�� 10�� ���⿡ �ٿ���
    // std::cout << 10 << std::endl; �̷��� �ڵ带 ��������
    // �������� �ϰ� �ȴ�.
    std::cout << COUNT << std::endl;

    // std::cout << "PrintDefine" << std::endl;
    PRINTDEFINE;


    return 0;
}
