
#include <iostream>
#include <time.h>
#include "Quick.h"

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	rand();

	CQuickSort<int>	Quick;
	Quick.SetSortFunction(Sort);

	int	Array[20] = {};

	for (int i = 0; i < 20; ++i)
	{
		Array[i] = rand();
		std::cout << Array[i] << std::endl;
		//Quick.push(rand());
	}

	Quick.Sort(Array, 20);

	std::cout << "============= Sort =============" << std::endl;
	for (int i = 0; i < 20; ++i)
	{
		std::cout << Array[i] << std::endl;
	}

	

	return 0;
}
