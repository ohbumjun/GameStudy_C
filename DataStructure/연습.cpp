#include <iostream>

template <typename T>
void wrapper(T&& u) {
	g(std::forward<T>(u));
}

class A {};

void g(A& a) { std::cout << "������ ���۷��� ȣ��" << std::endl; }
void g(const A& a) { std::cout << "������ ��� ���۷��� ȣ��" << std::endl; }
void g(A&& a) { std::cout << "������ ���۷��� ȣ��" << std::endl; }

int main() {
	A a;
	const A ca;

	std::cout << "���� --------" << std::endl;
	g(a);
	g(ca);
	g(A());

	std::cout << "Wrapper -----" << std::endl;
	wrapper(a);
	wrapper(ca);
	wrapper(A());
}
