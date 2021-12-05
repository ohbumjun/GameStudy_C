// Q. this 정의 
// - 객체를 참조하는 포인터. 즉, 객체 자신의 주소값을 가리키는 포인터

// Q. Self-Reference 의미와 예제

// - 객체 자신을 참조할 수 있는 참조자 
// - this 포인터를 이용해서, 객체가 자신의 참조에 사용할 수 있는 참조자의 반환문을 구성할 수 있다. 
class SelfRef
{
public :
    void print_hello()
    {
        cout << "hello";
    }
    SelfRef& Adder(int n)
    {
        return *this;
    };
};

int main()
{
    SelfRef obj;
    SelfRef &ref = obj.Adder(4);
    ref.print_hello();
};

// Q. 아래 대입 연산 과정에서, 참조자 ref 에 전달되는 값 ?
int main()
{
    int num = 7;
    int &ref = num;
    // 한가지 확실한 것은, "num에 저장된 정수값"이 전달되는 것은 아니다
    // "변수 num을 참조할 수 있는 참조의 정보가 전달된다."
    // 변수 num을 참조할 수 있는 참조 값이, 참조자 ref에 전달되어, ref가 변수 num을 참조하게 된다. 
}