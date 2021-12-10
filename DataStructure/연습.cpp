#include <iostream>

using namespace std;

template<typename T>
T Add(const T& n1, const T& n2)
{
	return n1 + n2;
}

int main()
{
    int n = Add<int>(1,2);
	double d = Add<double>(1.1, 2.2);
	cout << n << endl;
	cout << d << endl; 
}
