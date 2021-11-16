#include<iostream>

using namespace std;

template<typename T>
struct Property
{
	T value;
	Property(T value)
	{
		// ���� ���� ������ 
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
		// ���� ������ �߰��� 
		// Ư�� ������ �߰��ϰ� �ʹٸ�
		// ���� �ڵ忡 �߰��Ѵ�.
		// ��, �̷� ���� ���̴�
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