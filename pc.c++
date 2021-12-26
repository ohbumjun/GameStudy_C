

template<typename T1, typename T2>
class A
{
    T1 n;
    T2 n;
};

template<typename T1>
class A<T1, int>{}


// 클래스 템플릿 매개변수로 , 값을 넘겨받을 수 있는 변수를 선언하면
// 넘겨받은 값에 따라서, 서로 다른형의 클래스를 만들어낸다.
