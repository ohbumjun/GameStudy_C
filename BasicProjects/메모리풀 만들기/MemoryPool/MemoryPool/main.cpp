#include <iostream>

// 우리의 목적은 하나다.
// lvalue로 들어오면, lvalue 참조로
// rvalue 참조로 들어오면 rvalue 참조로 
// 들어온 데이터를 있는 그대로 넘겨주는 것이 중요하다.
// 아래와 같이 forward를 쓰면, template 함수 하나만으로
// 둘의 기능을 구현하는 것이 가능하다.
// 만약 move만을 사용해야 한다면
// 2개 이상의 함수를 제작해야 한다

template <typename T>
void print(T arg) {
	std::cout << arg << std::endl;
}

template<typename T, typename ...Types>
void print(T arg, Types ...args)
{
	std::cout << arg << ". ";
	print(args...);
}

int main() {
	print(1, 3.1, "abc");
	print(1, 2, 3, 4, 5, 6, 7);
	return 0;
}
