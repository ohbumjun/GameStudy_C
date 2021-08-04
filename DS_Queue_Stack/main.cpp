
#include <iostream>
#include "Stack.h"
#include "Queue.h"
#include "CQueue.h"

/*
Queue, Stack
Stack : 선입 후출의 자료구조이다. 먼저 들어간 데이터가 나중에 나오는 구조이다.
Queue : 선입 선출의 자료구조이다. 먼저 들어간 데이터가 먼저 나오는 구조이다.
*/

int main()
{
	CCircleQueue<int, 10>	Queue;

	for (int i = 0; i < 5; ++i)
	{
		Queue.push(i);
	}

	for (int i = 0; i < 3; ++i)
	{
		std::cout << Queue.front() << std::endl;
		Queue.pop();
	}

	for (int i = 0; i < 10; ++i)
	{
		Queue.push(i);
	}

	std::cout << "====" << std::endl;

	while (!Queue.empty())
	{
		std::cout << Queue.front() << std::endl;
		Queue.pop();
	}

	/*CQueue<int>	Queue;

	for (int i = 0; i < 100; ++i)
	{
		Queue.push(i);
	}

	while (!Queue.empty())
	{
		std::cout << Queue.front() << std::endl;
		Queue.pop();
	}*/

	/*int	Array[100] = {};

	CStack<int>	stackInt;

	for (int i = 0; i < 100; ++i)
	{
		Array[i] = i + 1;
	}

	for (int i = 0; i < 100; ++i)
	{
		stackInt.push(Array[i]);
	}

	for (int i = 0; i < 100; ++i)
	{
		Array[i] = stackInt.top();
		stackInt.pop();
	}

	for (int i = 0; i < 100; ++i)
	{
		std::cout << Array[i] << std::endl;
	}*/

	/*while (!stackInt.empty())
	{
		std::cout << stackInt.top() << std::endl;
		stackInt.pop();
	}*/
	return 0;
}
