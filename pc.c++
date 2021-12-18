#include <iostream>
#include <list>

using namespace std;

template<typename T1, typename T2>
void Add(T1 elem1, T2 elem2)
{
    cout << elem1 + elem2;
};

template<typename T1, int T2>
void Add(T1 elem, int T2)
{
    cout << elem + T2 + 10;
};

int main()
{
    Add<char>('c', 1);
    return 0;
}


