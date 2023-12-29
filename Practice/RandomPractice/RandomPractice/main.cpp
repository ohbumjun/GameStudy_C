#include <initializer_list>
#include <iostream>
#include <string>

class A
{
public:
	A() { std::cout << "C" << std::endl; }
	A(A& a) { std::cout << "&" << std::endl; }
	A(A&& a) noexcept { std::cout << "&&" << std::endl; }
	~A() { std::cout << "D" << std::endl; }

	void hello(){}
};

class childA : public A
{
public :
	void hello() {}
};

A F1(A a) {
	return a;
};

A F2(A& a) { return a; }

int main()
{
	A a;
	std::cout << "-------" << std::endl;
	A b = a;
	std::cout << "-------" << std::endl;
	A c = F1(b);
	std::cout << "-------" << std::endl;
	A d = F2(c);

	return 0;
}
// 컴파일 결과 ?