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
	CParent(){}
	~CParent(){}
public :
	virtual void Outputpure() = 0; 
	virtual void Outputpure1() = 0
	{
		cout << "hello" << endl;
	}
};

class CChild : public CParent
{
public:
	CChild() {}
	~CChild() {}
public:
	virtual void Outputpure(){}
	virtual void Outputpure1()
	{
		cout << "CChild" << endl;
	}
};
