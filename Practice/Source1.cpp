
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <algorithm>
#include "sample.h"

bool Sort(const int& Left, const int& Right)
{
	return Left > Right;
}

int main()
{
	srand((unsigned int)time(0));
	rand();

	//qsort()

	LARGE_INTEGER	PrevTick;
	LARGE_INTEGER	Tick;
	LARGE_INTEGER	SecondTick;

	QueryPerformanceFrequency(&SecondTick);

	CMergeSort<int>	Merge;
	Merge.SetSortFunction(Sort);

	int* Array = new int[100000];
	int* Array1 = new int[100000];
	int* Array2 = new int[100000];

	for (int i = 0; i < 100000; ++i)
	{
		Array[i] = rand();
		Array1[i] = Array[i];
		Array2[i] = Array[i];
		//std::cout << Array[i] << std::endl;
	}


	QueryPerformanceCounter(&PrevTick);
	Merge.Sort(Array, 100000);
	std::cout << "sort end" << std::endl;

	QueryPerformanceCounter(&Tick);

	float	Time = (Tick.QuadPart - PrevTick.QuadPart) / (float)SecondTick.QuadPart;

	std::cout << "Merge : " << Time << std::endl;

	std::cout << "============= Sort =============" << std::endl;
	/*for (int i = 0; i < 20000; ++i)
	{
		std::cout << Array[i] << std::endl;
	}*/



	return 0;
}
