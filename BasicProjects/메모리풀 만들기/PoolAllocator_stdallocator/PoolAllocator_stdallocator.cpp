#include <iostream>
#include <string>
#include "PoolAllocator_Brotcrunsher.h"

struct Person
{
	int m_age;
	std::string m_name;
	std::string m_haircolor;

public :
	Person(int age, std::string name, std::string haircolor)  :
		m_age(age), m_name(name), m_haircolor(haircolor){}

	void print()
	{
		std::cout << m_name << " " << m_age << " " << m_haircolor << std::endl;
	}
};

int main()
{
	bbe::PoolAllocator<Person> personAllocator;

	Person* p1 = personAllocator.allocate(18, "Peter", "Braun");
	Person* p2 = personAllocator.allocate(20, "Peter", "Blond");

	p1->print();
	p2->print();

	personAllocator.deallocate(p1);
	personAllocator.deallocate(p2);

	return 0;
}