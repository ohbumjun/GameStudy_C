// Q. template<typename T>, template<> 은 각각 언제 사용하는 것일까 

template<typename T>
class Simple
{
public :
    T SimpleFunc(T num){}
};

template<>
class Simple<int>
{
public :
    int SimplFunc(int num){}
};

// "정의" 부분에 T가 존재하면, T에 대한 설명을 위해서 template<typename T>
// 의 형태로 덧붙이면 되고, T가 존재하지 않으면, template<> 으로만 세팅 

// Q. 템플릿 static 멤버변수 예시
template<typename T>
class SimpleStatic
{
private :
    static T mem;
};

template<typename T>
T SimpleStatic<T>::mem = 0;

// Q. 템플릿 static 멤버변수 초기화의 특수화 
template<typename T>
T SimpleStatic<T>::mem = 0; // long 형태로

template<>
long SimpleStatic<long>::mem = 0;