
#include <iostream>
#include "List.h"
#include <time.h>

/*
�ڷᱸ�� : �����͸� �����ϴ� ���.
*/

// typedef : Ÿ���� �ٸ� �̸����� ������ ���ִ� ����� �����Ѵ�.
typedef int MyInt;

bool SortInt(const int& Left, const int& Right)
{
	return Left < Right; //
}


// F5 �� ������ �ϸ� Debuging ���� ������ �ȴ�.
int main()
{
	srand((unsigned int)time(0));
	rand();

	//MyInt	Test = 100;
	CList<int>		listInt;
	CList<float>	listFloat;

	for (int i = 0; i < 100; ++i)
	{
		listInt.push_back(i);
	}

	std::cout << listInt.back() << std::endl;
	std::cout << listInt.front() << std::endl;

	CList<int>::iterator	iter;

	iter = listInt.erase(80);

	std::cout << "Delete Next Node Data : " << *iter << std::endl;

	listInt.clear();

	for (int i = 0; i < 100; ++i)
	{
		listInt.push_back(rand() % 1000);
	}


	std::cout << "======= Before =======" << std::endl;
	for (iter = listInt.begin(); iter != listInt.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}

	listInt.sort(SortInt);

	std::cout << "======= After =======" << std::endl;
	for (iter = listInt.begin(); iter != listInt.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}

	std::cout << "======= Reverse =======" << std::endl;
	CList<int>::reverse_iterator	riter;

	for (riter = listInt.rbegin(); riter != listInt.rend(); ++riter)
	{
		std::cout << *riter << std::endl;
	}

	//listInt.pop_back();

	// const ��ü�� �Ϲ� ����Լ� ȣ���� �Ұ����ϴ�.
	// �Լ� �ڿ� const�� �پ��ִ� �Լ��� ȣ���� �����ϴ�.
	//const CList<float>	listFloat1;
	//listFloat1.push_back(10.1f);

	return 0;
}
