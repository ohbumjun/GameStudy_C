#include <iostream>

int& function1()
{
	int val = 10;
	return val;
}

int& function2()
{
	int val = 20;
	return val;
}

int main()
{
	int &ref1 = function1();
	int &ref2 = function2();
	std::cout << ref1;
}
