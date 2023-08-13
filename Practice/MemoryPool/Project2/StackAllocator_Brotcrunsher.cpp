/*
 *
 #include <iostream>
#include "StackAllocator_Brotcrunsher.h"

using namespace std;

struct Person {
	void print()
	{
		cout << "hello !" << endl;
	}
};

int main()
{
	bbe::StackAllocator personStackAllocator;

	auto marker = personStackAllocator.getMarker();
	Person* ps1 = personStackAllocator.allocateObject<Person>(10, -1, "undefined");

	for (size_t i = 0; i < 10; ++i)
	{
		ps1[i].print();
	}

	personStackAllocator.deallocateToMarker(marker);
	// personStackAllocator.deallocateAll();
}
 */