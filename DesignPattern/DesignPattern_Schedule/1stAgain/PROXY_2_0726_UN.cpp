
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

class String
{
    int *ref;
    char *buff;

public:
    String(const char *s)
    {
        buff = new char[strlen(s) + 1];
        strcpy(buff, s);
        ref = new int(1);
    }
    String(const String *s) : buff(s.buff), ref(s.ref)
    {
        ++(*ref);
    }
    // [] 리턴값으로 char을 대신할 객체 설계한다
    class CharProxy
    {
        String *s;
        int index;

    public:
        CharProxy(String *p, int n) : s(p), index(n) {}
        // CharProxy는 char로 변환될 수 있어야 한다.
        // 변환연산자가 필요하다
        // 아래 코드는 char 형으로 변환하는 형변환 코드이다
        // 즉, 형변환 연산자 overloading
        operator char()
        {
            cout << "읽는 작업 중 자원분리될 필요없다" << endl;
            return s->buff[index];
        }
        // CharProxy는 char를 대입할 수 있어야 한다
        // 왜 꼭 CharProxy 형태를 return 해야 하는가 ?
        // 대입연산자는 void x , 특정 형태를 반드시
        // return 시켜줘야 한다.
        CharProxy &operator=(char c)
        {
            cout << "쓰는 작업 중, 자원분리 후 대입" << endl;
            s->buff[index] = 'c';
            return *this;
        }
    };
    CharProxy operator[](int index)
    {
        cout << "operator []" << endl;
        return CharProxy(this, index);
    }
} int main()
{
    String s1 = "hello";
    String s2 = s1;
    // [] 연산을 통해 String을 읽고,쓰는 과정을
    // CharProxy를 통해 구현
    char c = s1[0]; // CharProxy 반환
    s1[0] = 'x';
    return 0;
}