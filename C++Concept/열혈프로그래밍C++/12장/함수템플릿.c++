// Q. 함수 템플릿 이란 

// - 함수를 만들어내는 도구

// Q. 아래코드에서 발생하는 일 
template<typename T>
T Add(const T& n1, const T& n2)
{
	return n1 + n2;
}

int main()
{
    int n = Add<int>(1,2);
	double d = Add<double>(1.1, 2.2);
    double d2 = Add<double>(1.1, 2.2);
	std::cout << n  << std::endl;
	std::cout << d  << std::endl; 
    std::cout << d2 << std::endl; 
};

// T를 int, double 로 해서 Add 함수를 1번씩 만든다
// - 템플릿으로 지정한다고 하여, 매 호출 문장마다 함수를 만들게 되는 것이 아니다
// - 즉, Add<double>(1.1, 2.2); 여기에서도 T를 double러 하는 Add함수를 만드는 것이 아니다
// - 이미 컴파일 할때 만들어놓은 함수를 호출할 뿐, 새 함수를 만들지 않는다.
// - 즉, 함수는 자료형당 하나씩만 만들어지는 것이다. 

// Q. 함수 템플릿과 속도

// 컴파일 할때 함수가 만들어진다. 속도가 감소하지만, 컴파일 속도가 감소하는 것이지 실행 속도가 느려지는 것은 아니다

// Q. 함수 템플릿과 템플릿 함수 차이 ?
template<typename T>
T Add (T n1, T n2)
{
    return n1 + n2;
}

// 템플릿 함수는, 함수 템플릿을 기반으로 컴파일러가 만들어내는 함수를 의미한다.
// 다른 말로 하면, 컴파일러에 의해 생성된 함수.
int Add (int n1, int n2)
{
    return n1 + n2;
}

// Q. 둘 이상의 type에 대한 템플릿 예시 

template<typename T1, typename T2>
void Print()
{
    cout << (T1)num1 << (T2)num2 << endl;
}


// Q. 함수 템플릿 특수화 정의 + 문자열 길이 , 사전 편찬 순서 비교함수 만들어보기

// 함수 템플릿 특수화란 ,상황에 따라 템플릿 함수 구성방법에 예외를 두는 방법을 의미한다.
template<typename T>
bool Max(T n1, T n2)
{
    return n1 > n2 ? n1 : n2;
};

template<>
bool Max(const char* s1, const char* s2)
{
    return strlen(s1) > strlen(s2) ?  true : false;
};

template<> // 사전 편찬 순서 
bool Max(const char* s1, const char* s2)
{
    // s1 < s2인 경우 음수 반환 
    return strcmp(s1, s2) > 0 ? true : false;
};