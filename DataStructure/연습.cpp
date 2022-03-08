#include <iostream>

template <int N>
struct Int {
  static const int num = N;
};

int main()
{
	std::cout << Int<1>::num;
	return 0;
}
