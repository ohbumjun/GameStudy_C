#include<iostream>
#include "Heap.h"
#include<time.h>

// �� 
// �ּ��� : �������� -- �ִ��� : �������� 
// ����Ʈ���� �����ϴ� ����� �޶�����
// ����Ʈ���� �����, �迭�ε� ������ �� �ִ�
// [Root][Root_L][Root-R][Root-_LL][Root_LR][Root_RL][Root_RR]

// ��������, ���⼭�� �迭���. ������ ���̴�
// ( �ִ� �� )
// Ư¡ : ���� �������� ����ִ´�. push_back ���� ��

// Ʈ�������δ�, ��ġ �Ʒ����ٰ� �޾��ִ� ����
// ���ʿ��� ������ , ��, ���� �Ʒ����� ����� ���鼭
// �޾��ִ� �����̴�. 

// �θ𺸴� �߰��� �ְ� �� ũ��, ��ġ�� �ٲ۴�
// ���� ū�ְ� ��Ʈ������ ��ġ�ϴ� ���� 

// �׷��ٸ�, �ִ������� "����"�� �Ѵٸ� ??
// "��Ʈ" �� ��ġ�� �ִ�,
// ���� ū.�ְ� ��������
// ���� ������ �Ʒ��� ��ġ�� �ָ� 
// �ش� ��Ʈ ��忡 �ø���

// ���� �ڽ� ����� ���ذ���
// ����,������ �ڽ��� �Ѵ� ũ�ٸ� 
// �� ū �ڽİ� �ٲ��ִ� �����̴�. 
/// (�ִ���.������ ū�ָ� ���� �ø���)

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	rand();

	CHeap<int> heap;
	Quick.SetSortFunction(Sort);

	int Array[20] = {};

	for (int i = 0; i < 20; i++)
	{
		Array[i] = rand();
		std::cout << << std::endl;
	}

	for (int i = 0; i < 20; i++)
	{
		heap.push(rand());
	}

	while (!heap.empty())
	{
		std::cout << heap.top() << std::endl;
		heap.pop();
	}

	return 0;
}