#include <iostream>

class Base {
public:
    virtual ~Base() {}

    void callVirtualFunction() const {
        // Call the protected virtual function from a public function
        virtualFunction();
    }

private:
    virtual void virtualFunction() const {
        std::cout << "Base::virtualFunction()" << std::endl;
    }
};

class Derived1 : public Base {
public :
    int a = 1;
private:
    void virtualFunction() const override {
        std::cout << "Derived1::virtualFunction()" << std::endl;
    }
};
int main() {
    Derived1 *d1 = new Derived1;
    Derived1 *d2 = new Derived1;
    // d2->a = 4;

    if (memcmp(d1, d2, sizeof(Derived1)) == 0) {
        std::cout << "equal." << std::endl;
    }
    else {
        std::cout << "not equal." << std::endl;
    }

    return 0;
}