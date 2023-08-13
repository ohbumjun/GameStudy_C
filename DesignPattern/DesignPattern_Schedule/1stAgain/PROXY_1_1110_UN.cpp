// 프록시 패턴
// 아무리 어려운 문제도
// 대신할 객체를 도입하면
// 해결할 수 있다

// 정리
// 1) 실제 Instance 와 같은 Interface 따르고
// 2) 실제 Instance 에 대한 Reference를 유지하고
// 3) 실제 Instance 에 대한 Access 를 control 한다

// 문제
// --> 참조계수로 동작하는
// string 클래스를 생각해봅시다
// COW(Copy On Write)
// 기법을 활용하여 String을 작성해봅시다

// COW 기법 ?
// s1,s2 라는 변수가 "hello"라는 문자열을 read 할때
// 원래는 s1,s2 각각 메모리 공간을 할당하고
// "hello" 라는 값을 주어줘야 한다.

// 하지만,
// 같은 대상을 "read"만 할 경우, 하나의 메모리 공간을 할당후
// s1,s2가 동일한 대상을 가리키게 하면 된다

// 그러닥, 둘중 하나라도 read 이상의 write까지 하게 될 때
// 그때 비로소 별도의 메모리 공간을 할당해준다
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

using namespace std;

class String
{
    int *ref;
    char *buff;

public:
    String(const char *s)
    {
        buff = new char[strlen(s) + 1];
        // 2번째 인자 : buff가 가리키는 메모리의 크기
        strcpy_s(buff, strlen(s) + 1, s);
        ref = new int(1);
    }
    // 복사 생성자 : 자기와 동일한 type의 instance가 들어오는 것
    String(const String &s) : buff(s.buff), ref(s.ref)
    {
        // *ref : 해당 buff를 참조하고 있는 개수
        ++(*ref);
    }
    // [] 연산자 : 객체를 배열처럼 보이게 하기
    char &operator[](int index)
    {
        cout << "operator []" << endl;
        return buff[index];
    }
    friend std::ostream &operator<<(std::ostream &out, const String &string)
    {
        cout << string.buff << endl;
    }
    friend ostream &operator<<(ostream &out, String &s)
    {
        return out << s.buff << endl;
    }
};

// 즉, 일반 string 에 대해서
// 중간에 String이라는 객체를
// Proxy로 두고 접근하는 원리
int
main()
{
    // 첫번재 줄은 "대입"연산자
    // 두번째 줄은 "복사"생성자
    String s1 = "hello";
    String s2 = s1; // copy initializer --> 이 경우, copy constructor가 호출된다.

    cout << s1 << endl;

    // 읽는 코드 : 자원은 계속 공유되어야 한다.
    char c = s1[0];
    // 쓰는 코드 : 이순간 자원은 분리된다
    s1[0] = 'x';

    // 그런데, 쓰는순간 자원이 분리되지 않는 것을 
    // 확인할 수 있다 ... (맞나)
    cout << s1 << endl; // xello
    cout << s2 << endl; // xello

    return 0;
}