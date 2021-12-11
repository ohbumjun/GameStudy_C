#include<iostream>

class Adder
{
public :
	void operator () (int a, int b) 
	{
		std::cout << a + b;
	}
};

int main()
{
	Adder a;
	a(1,2);
	return 0;
}
