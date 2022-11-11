#include <iostream>
#include <mutex>  // mutex 를 사용하기 위해 필요
#include <thread>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

#define MAX 1000001

bool IsDecimal[MAX];

int main() {

    memset(IsDecimal, true, sizeof(IsDecimal));

    int M, N;

    cin >> M >> N;

       cout << "hl" << endl;

    int minN = min(M, N);
    int maxN = max(M, N);

    for (int i = 2; i < sqrt(1000001); ++i)
    {
        if (IsDecimal[i] == false)
            continue;

        for (int j = i; j < MAX; j += i)
            IsDecimal[j] = false;

    }

    IsDecimal[2] = true;
    IsDecimal[3] = true;

    for (int i = minN; i <= maxN; ++i)
    {
        if (IsDecimal[i])
            cout << i << " ";
    }

    cout << endl;
    return 0;
}
