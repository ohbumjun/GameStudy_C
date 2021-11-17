#include <iostream>

using namespace std;

template<typename T>
struct Property
{
private :
	T value;
public :
	Property(T new_value)
	{
		*this = value;
	}
	operator T()
	{
		return value;
	}
	T& operator = (T new_val)
	{
		return value = new_val;
	}
};

struct Creature
{
	Property<int> strength{ 10 };
	Property<int> agility{ 10 };
};

int main()
{
	Creature c;
	c.strength = 10;
	int x = c.agility;
	return 0;
}