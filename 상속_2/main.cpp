/*
1) ���������Լ� ����
2) ���������Լ� Ư¡ 2����
3) �߻� Ŭ���� ���� �� Ư¡

*/

/*
1) �����Լ� �ڿ� =0�� �ٿ��ָ�
�ش� �����Լ��� ���������Լ��� �ȴ�.

2) 
- �ڽ� Ŭ�������� ������ �����Ǹ� �ؾ� �Ѵ�.
���� �ڽ� Ŭ�������� �����Ǹ� ���ߴٸ�
�ش� �ڽ� Ŭ������, �߻� Ŭ������ �Ǿ� 
�ش� Ŭ���� Ÿ���� ��ü ������
�Ұ����ϴ�
- ���������Լ��� �ش� ���������Լ���
������ִ� Ŭ������
�����κ��� �ݵ� �ǰ�, �ȵ� �ȴ�.
3) �߻� Ŭ����
- ���������Լ��� ������ �ִ� Ŭ������
�߻�Ŭ������� �Ѵ�.

�߻�Ŭ������ ��ü������ �Ұ����ϴ�. 
*/

#include<iostream>
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
		cout << "CParent Output" << endl;
	}
	virtual void OutputPure() = 0;
	virtual void OutputPure1() = 0
	{
		cout << "CParent OutputPure1" << endl;
	}
};

class CChild : public CParent
{
public :
	CChild()
	{

	}
	virtual ~CChild()
	{

	}
public :
	virtual void OutputPure() override
	{
		cout << "CChild Outputpure" << endl;
	}
	virtual void OutputPure1() override
	{
		cout << "CChild Outputpure1" << endl;
	}
};

int main()
{
	CParent* Child = new CChild;
	Child->OutputPure();
	delete Child;
}