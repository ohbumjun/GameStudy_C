#include <iostream>

// �츮�� ������ �ϳ���.
// lvalue�� ������, lvalue ������
// rvalue ������ ������ rvalue ������ 
// ���� �����͸� �ִ� �״�� �Ѱ��ִ� ���� �߿��ϴ�.
// �Ʒ��� ���� forward�� ����, template �Լ� �ϳ�������
// ���� ����� �����ϴ� ���� �����ϴ�.
// ���� move���� ����ؾ� �Ѵٸ�
// 2�� �̻��� �Լ��� �����ؾ� �Ѵ�

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
