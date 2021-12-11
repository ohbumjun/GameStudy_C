// Q. 의도

// 클래스 템플릿을 특수화 하려는 이유는, 특정 자료형을 기반으로 생성된 객체에 대해
// 구분이 되는 다른 행동양식을 적용하기 위함 


// 예시
template<typename T>
class Simple
{
public :
    Simple(T num){...}
};

template<>
class Simple<int>
{
public :
    Simple(int num){...}
};

int main()
{
    Simple<int>(1);
}

// Q. 클래스 템플릿의 부분 특수화 예시 ---
template<typename T1, typename T2>
class Simple{};

template<typename T1>
class Simple<int, T1>{};

// 결정되지 않은 자료형의 수가 2개에서 1개로 줄어들고
// 다른 하나에 대해서만, 특수화를 적용한 것이다.

// Q. 템플릿 매개변수 변수 선언 예시 및 특성 ---
template<typename T1, int len>
class SSimple
{
public : 
    T1 m[len];
};

int main()
{
    SSimple<int,6> s1;
    SSimple<int,6> s1_c;
    s1 = s1_c; // 가능 
    SSimple<int,7> s2;
    SSimple<int,7> s2_c;
    s2 = s2_c; // 가능

    s1 = s2; // 불가능
}

// 왜냐하면, 템플릿 매개변수에 값을 전달받을 수 있는 변수를 선언하면
// 변수에 전달되는 상수를 통해서, 서로 다른 형의 클래스가 생성되게 할 수 있다.
// s1, s2는 다른 형의 클래스가 된다는 의미이다. 