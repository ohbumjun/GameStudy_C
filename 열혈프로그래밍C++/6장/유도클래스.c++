/*
Q. 유도 클래스 객체 생성과정 발생하는 일 2가지 

- 유도 클래스의 객체 생성 과정에서, 기초 클래스는 100% 호출된다
- 유도 클래스의 생성자에서 , 기초 클래스의 생성자 호출을 명시하지 않으면  
기초 클래스의 void 생성자가 호출된다.
*/

// 3가지 형태의 상속 
// Q. 접근 제어 지시자의 허용된 접근의 범위                                    ------
private < protected < public 


// Q. protected 상속 의미 + ( 아래 3개 변수가 유도 클래스에서 취하는 형태 )     ------
class Base
{
private :
    int  num1;
protected :
    int num2;
public :
    int num3;
};

// protected 상속의 의미
// - protected보다 접근의 범위가 넓은 멤버는, protected로 변경시켜서 상속하겠다 
class Derived : protected Base
{
// private : --> 접근 불가 
    int  num1;
protected :
    int num2;
// public : --> protected 
    int num3;
}

int main()
{
    Derived drv;
    // 컴파일 에러 발생 --> num3 를 protected로 상속했기 때문이다
    // protected로 상속했기 때문에, public 멤버변수인 num3는 Derived 클래스에서 protected 멤버가 된다.
    // 따라서, 외부에서는 접근이 불가능한 멤버가 될 것이다 .
    std::cout << drv.num3 << std::endl;
    return 0;
}

// Q. protected 상속 의미 + ( 아래 3개 변수가 유도 클래스에서 취하는 형태 )     ------
class Base
{
private :
    int  num1;
protected :
    int num2;
public :
    int num3;
};

// Q. private 상속의 의미                                                   ------
// - private 접근의 범위가 넓은 멤버는, private 변경시켜서 상속하겠다 
class Derived : private Base
{
// private : --> 접근 불가 
    int  num1;
// protected : --> private 
    int num2;
// public : --> private 
    int num3;
}

// private 상속이 이뤄진 클래스를 다시 상속한 경우 
// 멤버함수를 포함하여 모든 멤버가 "접근 불가"가 되기 때문에 사실상 의미없는 상속이 되고 만다. 
class Derived2 : private Derived
{
// private : --> 접근 불가 
    int  num1;
// protected : --> 접근 불가  
    int num2;
// public : --> 접근 불가  
    int num3;
}

// Q. public  상속의 의미
// - public 접근의 범위가 넓은 멤버는, public 변경시켜서 상속하겠다 
// 다른 말로 하면, private을 제외한 나머지는 그냥 그대로 상속한다. 


// Q. 상속 ~ IS.A 관계 ?
// 상소관계가 성립하려면, 기초 클래스와 유도 클래스 간의 is - a 관계가 성립해야 한다. 
// 컴퓨터 --> 노트북 컴퓨터
// 노트북 컴퓨터는, 컴퓨터.이다. 

// 