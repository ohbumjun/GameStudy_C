// Q. 예외 처리시 if문이 선호되지 않는 이유

// - 예외처리를 위한 코드와 , 프로그램의 흐름을 구성하는 코드를
// 쉽게 구분하지 못한다.
//- if문은 예외처리로도 활용되지만, 논리적인 기능의 완성을 위해서도 사용된다.
// if문만 보고, 예외처리군! 이라고 할수 없다. 

// Q. try, catch, throw 각각 의미 및 구성
// try : 예외 발견, catch : 예외 잡고, thorw : 예외 던지고 
try
{
    /* code */
    // 예외 발생 예상 지역
    if (예외 발생)
        throw expn; //예외 던진다. --> 역서 예외가 발생하면, 더이상의 try 내 코드를 실행 x, catch로 넘어감
    // 그외 코드 ~~
}
catch(type expn) // 예외 전달 
{
    std::cerr << e.what() << '\n';
}

// Q. 스택풀기 정의 + 이때 발생하는 일 

// "특정 함수 내에서, 발생한 예외를 처리해주지 않으면 
// 예외가 발생한 함수를, 호출한 영역으로 예외 데이터가 전달된다(더불어 예외처리 책임까지)"
// 이러한 현상을 "스택 풀기"라고 한다.

// 함수 내에서 함수를 호출한 영역으로, 예외 데이터를 전달하면, 해당 함수는 마치 return 문 처럼
// 해당 함수를 더 이상 실행하지 않고 , 종료시킨다.

// Q. 아래 실행 결과
const Divide(int num1, int num2)
{
    if (num2 == 0)
        throw num2;
    cout << "Divide 1" << endl;
}

int main()
{
    int i1, i2; // 1, 0 입력 
    cin >> i1 >> i2;
    try
    {
        Divide(i1, i2);
        cout << "main 1" << endl;
    }
    catch(const std::exception& e)
    {
        cout << "예외 발생 " << endl;
    }
    
}

// 답 : "예외 발생"

const Divide(int num1, int num2)
{
    if (num2 == 0)
        throw num2; // 예외 발생 --> main 함수내 Divide 함수 호출 위치로 돌아가고 
    cout << "Divide 1" << endl; // 이 문장은 실행되지 않는다.
}

int main()
{
    int i1, i2;
    cin >> i1 >> i2;
    try
    {
        Divide(i1, i2);
        cout << "main 1" << endl; // 예외를 처리할 책임이 넘어옴. try 문에서 예외 발생시
                                  // 이후 코드를 더이상 실행하지 않고, catch 로 넘어간다.
    }
    catch(const std::exception& e)
    {
        cout << "예외 발생 " << endl;
    }
}

// Q. 아래의 경우 발생 현상 
int SimpleFunc()
{
    try
    {
        if (...)
            throw -1; // int형 예외 전달 
    }
    catch(char expn)  // char형 예외 데이터를 전달 
    {
        std::cerr << e.what() << '\n';
    }
}

// 자료형의 불일치로 인해서, 예외는 처리되지 않는다
// catch 블록으로 전달되지 않는다
// 따라서 SimpleFunc 함수를 호출한 영역으로 예외 데이터가 전달된다.

// Q. 하나의 try, 다수의 catch 블록을 사용하는 경우

// 하나의 try 블록 내에서 자료형이 다른 둘 이상의 
// 예외 발생이 가능할 수 있다.
// 이를 처리하기 위해 둘 이사의 catch가 존재할 수 있다.
int SimpleFunc()
{
    try
    {
        if (...)
            throw -1; // int형 예외 전달 
        else if (...)
            throw 'x';
    }
    catch(int expn)
    {
        cout << "int 형 예외" << endl;
    }
    catch(char expn)  // char형 예외 데이터를 전달 
    {
        cout << "char 형 예외" << endl;
    }
};

// Q. 예외 명시하기 ?

// 함수 내에서 발생할 수 있는 예외의 종류도 함수의 특징으로 간주한다.
// 이를 명시해주는 것도 좋다
int Simple(int num) throw (int, char)
{...}
int main()
{
    try
    {
        Simple(1);
    }
    catch(int expn)
    {...}
    catch(char expn)
    {...}
}

// Q. 예외 클래스 정의 +예시 + 예외 클래스 상속 구조 

// 예외발생을 알리는데 사용되는 객체를 가리켜 "예외 객체"라고 한다.
// 예외 객체를 이용해서, 예외상황을 알리면, 예외가 발생한 원인에 대한 정보를
// 보다 자세히 담을 수 있다. 

class Exception
{
public :
    void ShowExp(){...}
};

class ExceptionSub : public Exception
{
public :
    void ShowExp(){...}
};

class ExceptionSub2 : public Exception
{
public :
    void ShowExp(){...}
};

int main()
{
    // 1번째 
    try
    {
        throw ExceptionSub(); 
    }
    catch(Exception& e)
    {
        e.ShowExp();
    }
    
    // 2번째 
    try
    {
        throw ExceptionSub2();
    }
    catch(Exception& e)
    {
        e.ShowExp();
    }

    // 이렇듯 둘 이상의 예외 클래스를 상속의 관계로 묶어 놓으면 
    // 예외 처리를 단순화 할 수 있다. 
}


// Q. catch 연달아 있을시 발생하는 전달 방식
try
{
}
catch(type1 p)
{
}
catch(type2 p)
{
}
catch(type3 p)
{
}

// 예외가 발생하면, 해당 예외 데이터를 전달받을 수 있는 catch 블록을
// 위에서 아래로 찾아내려가고
// 적절한 블록을 찾으면, 해당 catch 블록이 실행되면서
// 예외의 처리는, 완료가 된다.


// Q. 다음 코드처럼 catch block을 구성하면 안되는 이유

class A 
{
public :
    void show(){cout << "A" << endl;}
}
class B : public A
{
public :
    void show(){cout << "B" << endl;}
}
class C : public B
{
public :
    void show(){cout << "C" << endl;}
}

void Expn(int expn)
{
    if (expn == 1)
        throw A();
    if (expn == 2)
        throw B();
    if (expn == 3)
        throw C();
}

int main()
{
    try
    {
        Expn(3);
        Expn(2);
        Expn(1);
    }
    catch(A& expn)
        expn.show();
    };
    catch(B& expn)
        expn.show();
    };
    catch(C& expn)
        expn.show();
    };
};

// 모두 
// catch(A& expn)
//     expn.show();
// }; 에서 걸려 버린다. 모든 예외 객체가 A 클래스를 상속하기 때문이다.

// Q. 해결책 ---

    try
    {
        Expn(3);
        Expn(2);
        Expn(1);
    }
    catch(C& expn)
        expn.show();
    };
    catch(B& expn)
        expn.show();
    };
    catch(A& expn)
        expn.show();
    };


// Q. new 연산자의 예외 처리 경우

// 메모리 공간의 할당이 실패하면 bad_alloc 이라는 예외가 발생한다.
#include<new> // 이 안에 예외 클래스로써 메모리 공간의 할당이 실패했음을 알리는 의도로 선언됨
int main()
{
    int num = 0;
    try
    {
        while(1)
        {
            num += 1;
            cout << num << " 번째 메모리 할당" << endl;
            new int[10000][10000]
        }
    }
    catch(bad_alloc& bad)
    {
        cout << bad.what() << endl;
    }
};

// 계속 메모리를 할당하게 되면, cout << bad.what() << endl; 가 어느순간 실행된다. 

