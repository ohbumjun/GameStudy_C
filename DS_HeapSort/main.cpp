
#include <iostream>
#include <time.h>
#include "Heap.h"

using namespace std;

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	CHeapSort<int>	heap;
	heap.SetSortFunction(Sort);

	for (int i = 0; i < 20; ++i)
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
