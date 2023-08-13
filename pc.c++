#include <list>
#include <iostream>

using namespace std;

int main()
{
    list<int> l1{1,2,3};
    list<int> l2{4,5,6};
    l1.splice(l1.end(), l2);

    for (const auto& e : l1)
        cout << ", " << e;

    return 0;
}
