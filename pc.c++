#include<iostream>

template<typename T>
void print(T p)
{
    std::cout << p << endl;
}

template<typeame T, typename ...Types>
void print(T t1, Types ...Args)
{
    std::cout << t1 << ". ";
    print(...Args);
};

int main()
{
    print(1, 3.1, "abc");
    print(1, 2, 3, 4, 5, 6, 7);
    return 0;
}