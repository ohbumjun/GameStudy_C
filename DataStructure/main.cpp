
#include <iostream>
#include <time.h>
#include "List.h"

int Factorial(int Number)
{
	if (Number == 1)
		return 1;

	return Number * Factorial(Number - 1);
}

int FactorialTail(int Number, int Result)
{
	if (Number == 1)
		return Result;

	return FactorialTail(Number - 1, Result * Number); //
}

int FactorialTail(int Number)
{
	return FactorialTail(Number, 1);
}

void Output(const int& Key, const int& Value)
{
	std::cout << "Key : " << Key << " Value : " << Value << " => ";
}

int main()
{
	srand((unsigned int)time(0));
	rand();
	//std::cout << FactorialTail(5) << std::endl;
	CBinaryTree<int, int>	tree;

	//tree.insert(333, 333);

	/*for (int i = 0; i < 20; ++i)
	{
		int	Number = rand();
		tree.insert(Number, Number);
	}*/

	tree.insert(5, 5);
	tree.insert(3, 3);
	tree.insert(1, 1);
	tree.insert(4, 4);
	tree.insert(10, 10);
	tree.insert(8, 8);
	tree.insert(15, 15);
	tree.insert(9, 9);

	tree.erase(10);

	CBinaryTree<int, int>::iterator	iter;

	for (iter = tree.begin(); iter != tree.end(); ++iter)
	{
		std::cout << "key : " << iter->first << " value : " <<
			iter->second << std::endl;
	}

	iter = tree.Find(500);

	if (iter == tree.end())
		std::cout << "ã�� �����Ͱ� �����ϴ�." << std::endl;

	else
	{
		std::cout << "Find Key : " << iter->first << " Find Value : " <<
			iter->second << std::endl;
	}

	//tree.clear();
	tree.erase(5);
	tree.erase(1);
	tree.erase(4);
	//tree.erase(3);
	//tree.erase(8);
	//tree.erase(15);
	//tree.erase(9);

	system("cls");
	std::cout << "=========== PreOrder ===========" << std::endl;
	tree.PreOrder(Output);
	std::cout << std::endl;

	std::cout << "=========== InOrder ===========" << std::endl;
	tree.InOrder(Output);
	std::cout << std::endl;

	std::cout << "=========== PostOrder ===========" << std::endl;
	tree.PostOrder(Output);
	std::cout << std::endl;
	std::cout << std::endl;

	for (iter = tree.begin(); iter != tree.end(); ++iter)
	{
		std::cout << "key : " << iter->first << " value : " <<
			iter->second << std::endl;
	}

	return 0;
}