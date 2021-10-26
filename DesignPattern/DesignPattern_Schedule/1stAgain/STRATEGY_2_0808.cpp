// 같은 기능
// 단, 템플릿 기반으로 정책을 교체하자

// 이전 코드의 경우, runtime 때
// 전략을 교체하는 형태였다.
// 성능면에서 아쉽다.
// overhead가 발생한다.

// <참고> 오버헤드란 ?
// = 프로그램의 실행흐름에서 나타나는 현상 중 하나.
// 예를 들어, 프로그램의 실행 흐름 도중
// 동떨어진 위치의 코드를 실행시켜야 할 때 ,
// 추가적으로 시간,메모리 자원이 사용되는 현상
// ex) 프로그래밍 시 외부함수 사용할 때 나타남.
// -- 실행 흐름 도중에 끊기고, 외부함수 사용하러 이동
// -- 함수를 사용하기 위해 스택 메모리 할당
// -- 매개변수가 있으면, 대입연산까지 발생
// 즉 , 예상하지 못한 자원들이 소모되는 현상이 바로
// 오버헤드 현상이다.
// 오버헤드를 줄이고자 , 매크로 함수와, 인라인 ㅎ마수
// 를 사용하여 최적화를 진행하곤 한다.

// 따라서, template을 통해 컴파일 시간에
// 결정하게 하여, 성능을 개선한다.

#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

struct IValidator
{
    virtual bool validate(string s, char c) = 0;
    virtual bool iscomplete(string s) { return true; }
};

class LimitDigitValidator
{
    int value;

public:
    LimitDigitValidator(int n) : value(n) {}
    inline bool validate(string s, char c)
    {
        return s.size() < value && isdigit(c);
    }
    inline bool iscomplete(string s)
    {
        return s.size() == value;
    }
};

template <typename Validate = IValidator>
class LineEdit
{
    string data;
    Validate pValidator;

public:
    LineEdit(int size = 0) : pValidator(size) {}
    string getData()
    {
        data.clear();
        while (1)
        {
            char c = _getch();
            if (c == 13 && (pValidator.iscomplete(data)))
                break;
            if (pValidator.validate(data, c))
            {
                data.push_back(c);
                cout << c;
            }
        }
        cout << endl;
        return data;
    }
};

int main()
{
    LineEdit<LimitDigitValidator> edit(5);
    while (1)
    {
        string s = edit.getData();
        cout << s << endl;
    }
}