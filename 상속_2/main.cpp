#include<iostream>

/*
-------
�߻� Ŭ���� : ���������Լ��� ������ �ִ�
Ŭ������ �߻�Ŭ������� �Ѵ�.

-------
���������Լ� :
�����Լ� �ڿ� = 0�� �ٿ��ָ�
�ش� �����Լ��� ���������Լ��� �ȴ�.

���������Լ��� �ڽ�Ŭ�������� ������
�����Ǹ� �ؾ� �Ѵ�

���� �ڽ� Ŭ�������� �����Ǹ� ���Ѵٸ�
�ش� �ڽ� Ŭ������ �߻�Ŭ������ �Ǿ
�ش� Ŭ���� Ÿ���� ��ü ������ �Ұ����ϴ�.

-------
���������Լ��� �ش� ���������Լ��� ������ִ�
Ŭ������ �����κ��� ���� �ǰ�
�ȸ��� �ȴ�.

���������Լ��� �ڽ�Ŭ�������� ���� ������ �Ǿ�� �ϴ� ���
���������Լ��� ����
�ڽĿ��� ������ �����Ǹ� �Ͽ�
�����ϰԲ� ���� �� ����Ѵ�. 

*/

using namespace std;

class CParent
{
public :
	CParent()
	{

	}
	virtual ~CParent()
	{

	}
public :
	virtual void Output()
	{

	}
	// ���������Լ�
	virtual void OutputPure() = 0;

	// �����κ��� ���� ���� �ְ� ���� ���� �ִ�
	// �ٸ�, ���������Լ��� �����ϴ� ����
	// CParent�� �߻� Ŭ������ �ǰ�
	// CParent�� ���� ��ü�� ������ �� ���� �ȴ�.
	virtual void OutputPure1() = 0
	{
		cout << "CParent OuptutPure1" << endl;
	}
};

class CChild : public CParent
{
public:
	CChild()
	{
	}
	virtual ~CChild()
	{
	}
public:
	// �Ϲ� �����Լ� : �ڽ� Ŭ��������
	// �����Ǹ� �ص� �׸�, ���ص� �׸�
	virtual void Output()
	{
	}
	// �ٸ�, ���������Լ��� ������ ������
	virtual void OutputPure() = 0;

	virtual void OutputPure1() = 0
	{
	}
};


class CChild1 : public CParent
{
public:
	CChild1()
	{

	}
	virtual ~CChild1()
	{

	}
public:
	// �Ϲ� �����Լ� : �ڽ� Ŭ��������
	// �����Ǹ� �ص� �׸�, ���ص� �׸�
	// virtual void Output()

	virtual void OutputPure() = 0;

	virtual void OutputPure1() = 0
	{
	}
};


int main()
{


	return 0;
}