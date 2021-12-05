// Q. 다음 코드의 결과 및 이유
void SwapByRef(int *ptr1, int* ptr2)
{
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
};

// 값이 변한다. 
// 두개의 주소값을 받아서, 그 주소값이 참조하는 영역에 저장되는 값을
// 직접 변경하고 있다.

// Q. 아래는 Call by Ref ? Val ?
int * SimpleFunc(int* ptr)
{
    //...
}

// 둘다 될 수 있다.
// 일반적으로 위와 같은 형태는 Call By Ref라고 생각한다
// 하지만 
int * SimpleFunc(int* ptr)
{
    return ptr + 1; // 이것은 그저 call by value 이다
    // 주소값을 이용해서, 함수 외부에 선언된 변수에 접근하는 Call - by - Ref와는 거리가 멀다.
}

// Q. Call by Ref 정의, Call by Val 과의 구분 기중

// "주소 값을 전달받아서, 함수 외부에 선언된 변수에 접근하는 형태의 함수 호출 "
// 즉, 주소값이 전달된 것이 중요한 것이 아니라, 함수외부에 선언된 변수에 접근했느냐.
// 즉, 주소값이 참조의 도구로 사용되었느냐.가 중요한 기준이 되는 것이다. 

// Q. 아래 형태 변수가 의아한 이유 , 왜 허용되는지
void SwapByRef2(int &ref1, int &ref2)
{
    int temp = ref1;
    ref1 = ref2;
    ref2 = temp;
}

// 왜 의아 ?
// - 참조자는 선언과 동시에 초기화 되어야 하는데, 그렇지 않았기 때문에 의아하다.
// 왜 허용 ?
// - 매개변수는 함수가 호출되어야만 초기화가 진행되는 변수들이다
// - 즉, 위의 매개변수 선언은, 초기화가 이뤄지지 않은 것이 아니라
// - 함수 호출시 전달되는 인자로 초기화를 하겠다는 의미의 "선언"이다

// Q. 아래 함수들을 통해 일어나는 과정
int main()
{
    int val1 = 10;
    int val2 = 20;

    SwapByRef2(val1,val2);
    return 0;
}

// 함수를 호출하는 순간
// 참조자 ref1과 ref2 는 각각 val1, val2 라고 이름 붙여진 메모리 공간의 또 다른 이름이 된다.
// 즉, ref1, ref2로 val1, val2 라는 이름이 붙여진 메모리 공간에 접근할 수 있게 된 것이다
// 그리고 SwapByRef2 에서는 이 두 참조자를 통해 값의 교환과정을 거치기 때문에 실제 val1, val2 의 교환으로 이어진다


#include<iostream>
int main()
{
	int num1 = 5;
	int* ptr1 = &num1;
	int num2 = 10;
	int* ptr2 = &num2;

    // Q. ptr1과 ptr2를 대상으로 를 호출할 때, ptr1, ptr2 가 가리키는 대상이 바뀌도록 SwapRef2 정의
	SwapByRef(ptr1, ptr2);

	std::cout << num1 << std::endl;
	std::cout << num2 << std::endl;
	return 0;
}


// 포인터를 참조하는 형태의 참조자로 매개변수들을 정의힌다.
void SwapByRef(int*(& ref1), int*(& ref2))
{
	int temp = *ref1;
	*ref1 = *ref2;
	*ref2 = temp;
}

// Q. void HappyFunc(const int &ref) 의 의미 ? --- 
// 해당 함수 내에서, 참조자 ref를 이용한 값의 변경은 하지 않겠다

// Q; 아래 함수에서 일어나는 변환 ( 2가지 ) --- 
int& RefRetFuncOne(int &ref)
{
    ref++;
    return ref;
}

int main()
{
    int num1 = 1;
    int &num2 = RefRetFuncOne(num1);

    ++num1;
    ++num2;
    // 둘다 4
}

// RefRetFuncOne 에서 "참조형으로 반환된 값"을 "참조자"에 저장하면
// 참조의 관계가 하나  더 추가된다. 
// 즉, num2 라는 참조자는, num1 으로 이름붙여진 메모리 공간의 또 다른 이름이 된다
// num2 로도 해당 메모리에 접근할 수 있게 된다는 것이다. 

// 매개변수 참조자 ref는 지역변수와 동일한 성격을 지닌다.
// 즉, RefRetFuncOne 이 반환을 하면, 참조자 ref는 소멸된다.
// 하지만, 참조자는 참조자일 뿐,그 자체로 변수가 아니기 때문에,
// 참조자가 참조하는 변수는 소멸되지 않는다
// 즉, RefRetFuncOne 가 호출되는 동안 ref1 이 num1 이름의 메모리 공간의 이름이 되어
// ref1으로 해당 메모리 공간을 접근할 수 있게 되지만, 함수가 반환되면서
// ref1 참조자는 사라지게 된다는 것이다. 
// 동시에 참조자는 변수가 아니므로, 참조자가 사라진다고 하여, 참조자가 참조하는 변수는 그대로이다.

// Q. 아래 함수에서 일어나는 변환 ---
int& RefRetFuncOne(int &ref)
{
    ref++;
    return ref;
}

int main()
{
    int num1 = 1;
    // 이부분만 다르다 .
    int num2 = RefRetFuncOne(num1);

    ++num1; // 2
    num2 += 2; // 3
    // 둘다 4
}

// 참조형으로 반환은 되지만, 일반변수를 통해 반환값을 저장한다.
// 즉, 반환"값"만 받아서, 다른 메모리 공간에 할당하고, 그 메모리 공간의 이름을 "num2" 라고 이름 지은 것이다
// 따라서 위와 같이 num1과 num2는 전혀 다른 메모리 공간의 이름인 것이다. 


// Q. 아래와 위의 차이 
int RefRetFuncTwo(int &ref) // 반환값이 기본자료형 int
{
    ref++;
    return ref;
}

int main()
{
    int num1 = 1;
    // 이부분만 다르다 .
    int num2 = RefRetFuncTwo(num1);

    ++num1; // 2
    num2 += 2; // 3
    // 둘다 4
}

// 결과는 동일하다
// 하지만, 차이가 있다
// 반환형이 참조형인 RefRetFuncOne 의 경우, 변수, 참조자 형태로 모두 저장할 수 있다
int num2 = RefRetFuncOne(num1); // o
int &num2 = RefRetFuncOne(num1);// o

// 반면, 반환형이 기본자료형일 경우 , RefRetFuncTwo 의 반환값은 
// 반드시 "변후"에 저장해야 한다.
int num2 = RefRetFuncTwo(num1);  // o
int &num2 = RefRetFuncTwo(num1); // 


// Q. 아래 코드 출력 값, 원인, 주의점
#include<iostream>

int& function()
{
	int val1 = 10;
	return val1;
};

int& function2()
{
	int val2 = 20;
	return val2;
}

int main()
{	
	int &ref  = function();
	int &ref2 = function2();
	std::cout << ref; // 10이 아니라, 20이 출력된다.
	return 0;
} 

// main()이 실행되면서, int ref, int ref2 에 대한 메모리가 할당된다 ( 8 byte )
// 예를 들어, ref 와 ref2 가 1000, 1004 에 할당되었다고 가정하자
// function()이 실행되면서 , 지역변수 val이 메모리 1008에 추가적으로 할당된다고 해보자.
// 이제 ref는 참조형 반환으로 val의 위치를 참조하게 된다. 이때 가리키는 값은 10이다
// function()이 종료되고, 지역변수 val이 차지하는 메모리공간 1008은 사라진다.
// function2()가 실행되면서, val2의 메모리가 또 다시 추가적으로 1008에 할당된다
// (스택 형식으로 메모리가 쌓이면서 할당된다.)
// 중요한 점은, ref가 아직도 1008을 가리키고 있다
// val2의 값은 20이 대입되었으니 ref의 값도 20이 되는 것이다.

// 주의점 : 즉, 절대로, 참조자가 지역변수를 참조하지 않게 한다.

