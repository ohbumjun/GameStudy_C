#include <future>
#include <iostream>
#include <thread>

class A
{
public:
	A(int a) { std::cout << "h" << std::endl; }
	A(std::initializer_list<int> l) { std::cout << "initial h" << std::endl; }
	A(const A& a) { std::cout << "copy" << std::endl; }
	A(A&& a) { std::cout << "move" << std::endl; }
};

int some_task(int x) {
	std::cout << "task" << std::endl;
	return 10 + x;
}

int main() {

	A a{ 1 };
	A b = { 1,2,3 };
	A c{ 1,2,3 };

	return 0;
}