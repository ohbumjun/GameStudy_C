#include <iostream>
#include <mutex>  // mutex 를 사용하기 위해 필요
#include <thread>
#include <vector>
#include <string>

using namespace std;

int gstrcpy(char* src, char* dest)
{
    int sum = 0;

    for (int i = 0; i < sizeof(dest); ++i)
    {
        src[i] = dest[i];
    }
    return sum;
}

int main() {

    char cArra[] = "hello1";
    char cArra2[] = "hello2";

    gstrcpy(cArra, cArra2);

    cout << cArra << endl;

    return 0;
}

// Insertion Sort 알고리즘을 사용해서 작은 값이
// 앞으로 오도록 List를 정렬한 다음
// head 를 반환하는 함수를 만드세요
// (기본적으로 알고 있는 InsertionNode 의 반대를 출력한다고 생각하기)