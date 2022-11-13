#include <iostream>

using namespace std;

class Base {
    int x;
public:
    void set(int a) { x = a; }
};

class Derived : public Base {
    int x, y;
public:
    void set(int a) { x = a + 1; }
};

int main(void) {

    Derived D;

    cout << sizeof(D) << endl;

    return 0;
}