#include<iostream>
#include<time.h>
#include "CBianaryTree.h"

using namespace std;

int Factorial(int Number)
{
	// �Լ��� ���ᰡ �Ǿ�߸�
	// �ش� �Լ��� ���� �������
	// Stack �� �޸𸮵��� �������.

	// �׷���, �Լ� ���� �ȵǰ�
	// ��������� �����ϰ� �Ǹ� 
	// ��� stack�� �������� ��ϵ��� ���̰� �ȴ�. 
	if (Number == 1) return 1;
	return Number * Factorial(Number - 1);
}

// ���� ���
// : ����ȭ�� ����Լ� �����
// ���������� stack�� ���̴� �ͱ�����
// ��� �� ������ ����. ������ ������ �ϴϱ�
// ��, 
// 
int FactorialTail(int Number, int Result)
{
	if (Number == 1) return Result;
	// ���ڿ��ٰ� ����� ����� �־��ִ� ��
	// �̷��� �Ǹ�, ��ü �Լ��� ��������� �ٽ� ȣ��� ��
	// ������ ������ �����ϴ� �۾��� ��ġ�� �ʴ´�
	// �ӽú��� ����� + ����ϰ� + �����ϰ� 
	// ex) return Number + Factorial(Number - 1);
	// �̷� ������ ��ġ�� �� �ƴ϶�
	// ���ڿ��ٰ� ������� �־��ֱ⸸ �ϴ� ���̴� 
	return FactorialTail(Number - 1, Result * Number);
}

int FactorialTail(int Number)
{
	return FactorialTail(Number,1);
}


int main()
{
	srand((unsigned int)time(0));
	rand();

	CBinaryTree<int, int> tree;

	tree.insert(333, 333);
	for (int i = 0; i < 20; i++)
	{
		int Number = rand();
		tree.insert(Number, Number);
	}

	CBinaryTree<int, int>::iterator iter;

	for (iter = tree.begin(); iter != tree.end(); ++iter)
	{
		// iter-> �� �ϴ� ���� m_Node
		// �׷��� iter->()->first �̷� ������ ȭ��ǥ �ѹ� �� ����ؾ� �ϴ� �� �ƴѰ� ?
		// �´µ�, ���⼭ ->�� �ѹ� �����Ǹ鼭 ���� ���� operator�� �۵��Ѵٰ� �����ϸ� ���Ѵ� 
		cout << "key : " << iter->first << endl;
		cout << "value : " << iter->second << endl;
	}

	cout << Factorial(5) << endl;
	cout << FactorialTail(5) << endl;
	
	iter = tree.Find(333);

	if (iter == tree.end())
		cout << "ã�� �����Ͱ� �����ϴ�." << endl;
	else
	{
		cout << "Find Key :" << iter->first  << endl;
		cout << "Find Value :" << iter->second << endl;
	}

	return 0;
}

/*
< Ʈ�� >
>> �⺻ ����
�θ� ��� ����
- �� �༮�� �ڽ� ������ ����
- �ڽ��� �ڽĵ� ���� ... ������� ����

>> ��Ʈ ��� : ���� ����� ���

>> ����Ʈ��

>> ���� ���
: ���̻� �ڽĳ�尡 ���� x

--------------------------------
< ���� Ʈ�� >
>> �⺻ ����
- �ڽ� ��尡 �ִ� 2�������� ����
- ������ ����, ũ�� ������

>> ������ Ž���� �־ ����ȭ

>> 1) ��ȭ ���� Ʈ��
- ��� ������ �� ���ִ� ����

>> 2) ���� ���� Ʈ��
- 1,2�� ���� ����������
���� ���� Ʈ��

>> 3) ���� Ʈ��
- �������θ� ġ������ ����
- ���ĵ� �����Ͱ� ���ʴ�� ���� �Ǹ�
����Ʈ���� ���������
ex) 1234567 --> :: 1->2->3 .. ->7
(����) �ڰ� ���� ����Ʈ��
: �����͸� ���� ������,
�˾Ƽ� ����Ʈ�� ������ �����ش� .

>> ó�� �ƹ��͵� ���� ����Ʈ����
�����͸� �߰��ϸ�, �ش� ����
��Ʈ��忡 ����.
- ����, ������ ������ ū ���� ������
�ش� ��� ������
- ���� ���� ������, �ش� ��� ����

*/