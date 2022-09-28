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

// Insertion Sort 알고리즘을 사용해서 작은 값이
// 앞으로 오도록 List를 정렬한 다음
// head 를 반환하는 함수를 만드세요