#include <iostream>

void change(int* &p)
{
	int a = 10;
	p = &a; 
}

int main()
{
	int * ptr = NULL;
	change(ptr);
	printf("%d : ", *ptr);
	return 0;
}
