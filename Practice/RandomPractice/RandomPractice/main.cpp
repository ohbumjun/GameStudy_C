#include <iostream>
#include <bitset>
#include <string>

using namespace std;

template<typename T,
         typename = decltype(std::declval<T>().begin()),
         typename = decltype(std::declval<T>().end())>
void A(T& t){}

template<typename T>



int main() {

    unsigned int n = -1; 

    cout << bitset<8>(n) << endl;

    return 0;
}

// Insertion Sort �˰����� ����ؼ� ���� ����
// ������ ������ List�� ������ ����
// head �� ��ȯ�ϴ� �Լ��� ���弼��