// Q. 클래스 템플릿, 템플릿 클래스의 정의 ?


// 클래스 템플릿 : 템플릿으로 정의된 클래스
// 템플릿 클래스 : 클래스 템플릿을 기반으로 컴파일러가 만들어내는 클래스 



// 아래와 같이 헤더, 소스, main.cpp 파일을 각각 별개의 파일로 나누었다고 해보자

// Header
template<typename T>
class Point
{
private :
    T n1, n2;
public :
    Point(T p_n1, T p_n2);
};

// Source 
template<typename T>
Point<T>::Point(T p_n1, T p_n2) 
                : n1(p_n1), n2(p_n2)
{
}

// Main
int main()
{
    Point<int> p1(1, 2);
    Point<float> p1(1.1f, 2.2f);
    Point<double> p1(1.1, 2.2);
}

// Q. main.cpp 에서 헤더는 include 해줬는데, 소스파일은 include 안했을 때 발생하는 일 + 이유

// 컴파일 에러가 발생한다.
// - 컴파일은, 파일 단위로 이루어진다.
// - main.cpp 이 컴파일 될 때, 컴파일러는 총 3개의 템플릿 클래스.를 생성해야 한다. 
// 이를 위해서는 클래스 템플릿인, Point의 모든 것을 알아야 한다.
// 즉, 컴파일러에게 헤더 뿐만 아니라, 소스 파일에 담기 정보도 필요하다
// 그런데 소스파일 정보를 참조할 수 없어서, 컴파일 에러

// 자. main.cpp 과 소스파일.cpp 모두 동시에 컴파일 되는 것은 맞다
// 하지만, 둘은 서로 다른 소스파일이기 때문에, 파일 단위 컴파일 원칙에 의해
// main.cpp를 컴파일 할때, 소스파일.cpp을 참조하지않는다.
// #include "소스파일.cpp" 이라는 과정을 거쳐야 한다.

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

    return 0;
}