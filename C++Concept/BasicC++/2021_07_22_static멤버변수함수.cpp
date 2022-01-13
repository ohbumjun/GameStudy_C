#include<iostream>
/*
�Ϲ� �Լ��� ���� ����
- ���α׷� ����� ������, �޸𸮰� �����ȴ�
- �ֳ��ϸ�, �޸��� ������ ����(���������� ����)��
���� �����̴�
ex) stack : ��������
ex) heap  : �����Ҵ�

�Ϲ� �Լ� �Ӹ� �ƴ϶�,
class ������
static ��� ���� �� �Լ���
����ϴ� ��찡 �ִ�
*/

class CStatic
{
public :
	CStatic(){}
	~CStatic(){}
public :
	int m_iNumber;
	static int m_iNumberStatic;
public :
	void OutPut(){}

	// 1) static ����Լ�������
	// this�� ����� �� ����.
	// 2) �ٸ� ���� �ϸ�, �Ϲ� ���������
	// ����� �� ����.
	// 3) ���⿡���� static ���������
	// ����� �����ϴ�
	// 4) �Լ� ������ ���°�, �����Լ���
	// ������ ���·� ����� �����ϴ�
	static void OutPutStatic()
	{
		std::cout << "Output Static" << std::endl;
	}
};

// Ŭ���� static ���������
// �ݵ�� Ŭ���� �ܺο� ����κ��� ������־�� �Ѵ�.
// int�� ���, �⺻���� 0���� �ʱ�ȭ�Ǹ�
// ���ϴ� ���� �������־ �ȴ�.

// Ŭ������ static ��������� �� Ŭ������ �̿��ؼ�,
// ��ü�� �ƹ��� ���� �����ϴ���, 
// �޸𸮿� ��1���� �����ȴ�.
// �ݸ�, �Ϲ� ��������� ���, �����Ǵ� ��ü�� ����ŭ �޸𸮿� ������.
int CStatic::m_iNumberStatic;

// �̱��� ����
class CSingleton
{
	CSingleton(){}
	~CSingleton(){}
	CSingleton(const CSingleton&){}
	CSingleton operator=(const CSingleton&){}

	static CSingleton* m_pInst;
public :
	// ��� �Լ� vs ��� ����
	// ��� ������ ���, �����Ǵ� ��ü���� �ٸ� �޸𸮿� �����ȴ�
	// �ݸ�, ��� �Լ��� ���
	// �޸� �ϳ��� ��ġ�Ͽ�, �ϳ��� ��� �Լ��� ��� ��ü�� �����Ͽ� ���� �����̴�

	// ��� �Լ� vs static ��� �Լ�
	// static ��� �Լ���, static ��� ������ ����������
	// �����Ǵ� ��ü�� ���� �������
	// �� �ϳ��� �޸𸮿� �ö󰡼�, ��ü���� �����ϴ� �����̴�
	// �׷��ٸ�, ��� �Լ����� ���̴� �����ϱ� ?
	// static ��� �Լ���, this�� ����� �� ����.
	// �ݸ� ��� �Լ��� this�� ����� �� �ִ�.
	// this ��밡�� ���ο����� ���̰� �����ϴ� ���̴�. 
	static CSingleton* GetInst()
	{
		if (m_pInst == nullptr)
		{
			// �����Ϳ� ���� �Ҵ�����
			// 1) �� �޸𸮿� CSingleton ����ü �����Ҵ�
			// 2) �ش� �ּҸ� m_pInst ������ ������ ����
			// 3) SingleTon�̱� ������, ��ü �ϳ����� ���� ���
			m_pInst = new CSingleton;
		}
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst != nullptr)
			delete m_pInst;
	}
};
// nullptr�� 0
// �Լ������ͺ����� 0�� �־��� �Ͱ� ���� �ǹ� 
CSingleton* CSingleton::m_pInst = nullptr;

int main()
{
	// 1) ���� ��� : ��ü�� ���� ����
	CStatic std1;
	std1.m_iNumberStatic = 100;
	// 2) class�� ���� ����
	CStatic::m_iNumberStatic = 300;

	// �Լ� ������ ���°�, �����Լ��� ����� ������� ���ȴ�. 
	void(*pFunc)() = CStatic::OutPutStatic;
	// �Ϲ� ����Լ��� �����ʹ� ������� �ٸ���
	void(CStatic:: * pFunc1)() = &CStatic::OutPut;
	// �Ϲ� ����Լ��� �ݵ�� this�� �����Ǿ�� �Ѵ�
	// �׷��� ������ � ��ü�� this�� ����������
	// �ݵ�� ����Ǿ�� �Ѵ�
	(std1.*pFunc1)();

	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;
	std::cout << CSingleton::GetInst() << std::endl;

	CSingleton::DestroyInst();

	return 0;
}