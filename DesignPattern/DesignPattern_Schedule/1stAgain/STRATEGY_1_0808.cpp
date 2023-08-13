// 전략 패턴
// -- 동적으로 알고리즘을 교체할 수 있다
// -- 알고리즘 기반 인터페이스를 정의한 후,
// 각각 알고리즘 클래스는, 이를 구현하게 한다
// -- 다시 말해서, "전략"에 해당하는 class들의
// 추상 interface를 정의하고 ,
// 각각의 "client" 객체가 해당 "전략" class들을
// has ! 소유 하게 하는 원리이다

// 템플릿 메서드 패턴과 함께 가장 많이 사용되는
// 패턴 중 하나이다.

// 변해야 하는 것을, 다른 클래스로 뽑아낸다.
// 왜냐하면 교체가 가능해야 하기 때문이다.
// 인터페이스를 먼저 설계한다

// 문제 :
// LineEdit 즉, 실시간으로 입력 가능한 텍스트 입력기를
// 만들어보자 !
// => LineEdit엔 Validate를 두어 해당 입력문자가 타당한지 안한지 검사하고, 타당하지 않으면 출력해주지 않는다.
// => 예를들어 크기가 5이고, 숫자만 받을수있는 Validator를 제작해보자
// => OCP를 고려해서 코딩해보자

// 장점 : 동적으로 정책을 교체할 수 있다
// 단점 : inline이 안되니 느리다.

#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

// 전략 class의 interface
struct IValidator
{
    virtual bool validate(string s, char c) = 0;
    virtual bool iscomplete(string s) { return true; }
};

class LimitDigitValidator : public IValidator
{
    int value;

public:
    LimitDigitValidator(int n) : value(n)
    {
    }
    virtual bool validate(string s, char c)
    {
        return s.size() < value && isdigit(c);
    }
    virtual bool iscomplete(string s)
    {
        return s.size() == value;
    }
};

class LineEdit
{
    string data;
    // ----------------------------------------
    IValidator *pValidator;

public:
    LineEdit() : pValidator(0) {}
    // 알고리즘의 정책을 실시간으로 결정할 수 있다.
    void setValidator(IValidator *p) { pValidator = p; }

    string getData()
    {
        data.clear();
        while (1)
        {
            char c = getch();
            if (pValidator == 0 || pValidator->iscomplete(data))
                break;
            if (pValidator == 0 || pValidator->validate(data, c))
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
    LineEdit edit;
    LimitDigitValidator v(5);
    edit.setValidator(&v);
    while (1)
    {
        string s = edit.getData();
        cout << s << endl;
    }
}