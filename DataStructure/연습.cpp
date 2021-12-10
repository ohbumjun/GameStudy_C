#include <iostream>

using namespace std;

class Adder
{
public :
    int operator () (int n1, int n2)
    {
        return n1 + n2;
    }
};

int main()
{
    Adder add;
    cout << add(2,3) << endl;
}
