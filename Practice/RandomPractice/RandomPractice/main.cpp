#include <iostream>
#include <mutex>  // mutex 를 사용하기 위해 필요
#include <thread>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

struct A{
   
    // A(A&& a)
    // {
    //     cout << "Move copy" << endl;
    // }
   
    int i = 0;
};

// A returnA(A a)

A returnA()
{
    A a;
    return a;
}

int main() {

    // A pA;
    A pB = returnA();

   //auto two_i = [pA]() -> int {
   //    
   //    int a;
   //    a = pA.i;
   //
   //    return a;
   //};
   //
   //auto copyT = two_i;

    return 0;
}