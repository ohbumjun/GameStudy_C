#include <iostream>

using namespace std;

// Q. 특정 템플릿 클래스의 객체를 인자로 받는 일반함수 정의와 friend 선언 예시
template<typename T>
class Point
{
private :
    T n1, n2;
public :
    Point(T p_n1, T p_n2) : n1(p_n1) , n2(p_n2)
    {}
    friend Point<int> operator + (const Point<int>&, const Point<int>&);
    friend ostream& operator << (ostream& os, const Point<int>&);
};

Point<int> operator + (const Point<int>& ref1, const Point<int>& ref2)
{
    return Point<int>(ref1.n1 + ref2.n1, ref2.n1 + ref2.n2);
}

ostream& operator << (ostream& os, const Point<int>& ref)
{
    os << ref.n1 << endl;
    return os;
}

int main()
{
    Point<int> pos1(3,4);
    Point<int> pos2(1,2);

    Point<int> pos3 = pos1 + pos2;
    cout << pos3 << pos1 << pos2;

    return 0;
}
