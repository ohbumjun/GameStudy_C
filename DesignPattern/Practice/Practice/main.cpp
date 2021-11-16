#include<iostream>

using namespace std;

template<typename T>
struct Property
{
	T value;
	Property(T value)
	{
		// 복사 대입 연산자 
		*this = value;
	}
	// Instead of Getter 
	operator T()
	{
		// cout << "operator()" << endl
		return value;
	}

	// Instead of Setter
	T& operator = (T new_value)
	{
		// 대입 연산자 중간에 
		// 특정 동작을 추가하고 싶다면
		// 여기 코드에 추가한다.
		// 즉, 이런 구조 없이는
		cout << "AssignMent\n" << endl;
		return value = new_value;
	}
};

struct Creature
{
	Property<int> strength{ 10 };
	Property<int> agility{ 5 };
};

int main()
{
	Creature c;
	c.strength = 11;
	int x = c.agility;
	return 0;
}