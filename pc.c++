#include <iostream>
#include <list>

using namespace std;

int main()
{
    int num = 10;
    int *ptr = &num;
    int *(&rptr) = ptr;

    int **pptr = &ptr;
    int **(&rrptr) = pptr;
}

/* 
*/