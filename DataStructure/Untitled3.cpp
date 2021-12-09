#include <iostream>

using namespace std;

class AAA
{
private :
    int num;
public :
    AAA(int n = 0) : num(n)
    {
        cout << "A constructor" << endl;
    }
    AAA(const AAA& refA) : num(refA.num)
    {
        cout << "A copy consructor" << endl;
    }
    AAA& operator = (const AAA& refA) 
    {
        cout << "A = operator" << endl;
        num = refA.num;
    }
};

class B
{
private :
    AAA bA;
public :
    B(const AAA& refA) : bA(refA){}
};

class C
{
private :
    AAA cA;
public :
    C(const AAA& refA) 
    {
        cA = refA;
    }
};

int main()
{
    AAA obj(12);
    cout << endl;
    B obj2(obj);
    cout << endl;
    C obj3(obj);
    return 0;
}
