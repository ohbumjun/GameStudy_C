// Q. 객체 포인터 변수 정의 ---

// 객체의 주소값을 저장하는 포인터 변수 
 
// Q. 객체 포인터 변수의 특성, 예시 ---

// AAA형 포인터 변수는 AAA 객체 또는 AAA 를 직접 혹은 간접적으로 상속하는 모든 객체를 가리킬 수 있다
// 객체의 주소값을 저장할 수 있다.
Person* ptr;
ptr = new Person;

class Student : public Person{}
class PartTimeStudent : public Student{}

Person * ptr = new Student;
Person * ptr = new PartTimeStudent;
Student * ptr = new PartTimeStudent;


// Q. 객체 포인터 특성.이 가능한 이유 ?
// Student는 Person 이다
// Student는 PartTimeStudent 이다.

// 객체 지향에서는, 위의 문장들이 성립함(IS ~ A 관계가 성립함)으로 인해서,
// Student, PartTiemStudent 객체를 Person 객체의 일종으로 간주한다.
// 따라서, Person 포인터 변수를 통해 Stduent, PartTiemStudent 객체를 가리킬 수 있는 것
// PartTiemStudent 객체를 Student 객체의 일종으로 간주
// 따라서, Student 포인터 변수를 통해 PartTiemStudent 객체를 가리킬 수 있는 것

// Q. 함수 오버로딩(overloading)과 오버라이딩(over-riding) 차이 
// 오버로딩은 함수의 중복 정의, 오버라이딩은 함수의 재정의

// 함수 오버로딩은, 함수의 이름은 같되, 매개변수의 형태(개수 및 타입)이 다른 함수를 정의하는 것을 허용해주는 것
// 단, 반환값이 다른 것만으로는 함수 오버로딩이 허용되지 않는다
// 함수를 구분해주는 기준은, 매개변수의 형태이지, 반환값의 형식이 아니기 때문이다. 
void func(int i)
{
	cout << "func(int) is called." << endl ;
}

void func(char c)
{
	cout << "func(char) is called." << endl;
}

int main()
{
	func(3);
	func('a');
}


// 함수 오버라이딩은, 함수의 재정의
// 힘수를 유도,파생 클래스에서, "동알한 이름과 형태로" 재정의를 할 수 있게 해주는 기능 
// 중요한 점은, 기초 클래스와 동일한 이름의 함수를 유도 클래스에서 정의한다고 해서
// 무조건 함수 오버라이딩이 되는 것은 아니다. 매개변수의 자료형 및 개수가 다르면
// 이는 함수 오버라이딩이 되어, 전달되는 인자에 따라서 호출되는 함수가 결정된다
// 즉, 함수 오버로딩은 상속의 관계에서도 구성이 될 수 있다. 
class Base
{
public:
	void func()
	{
		cout << "B::func() is called" << endl;
	}
};

class Derived : public Base
{ 
public:
	void func()
	{
		cout << "D::func() is called" << endl;
	}
};

int main()
{
	Base b;
	Derived d;

	b.func();
	d.func();
}

// Q. Child 클래스에서 Parent 클래스를 오버라이딩 한 이유 ?
class Employee
{
private :
    int cost;
public :
    int GetPay()
    {
        return cost;
    }
    void ShowInfo()
    {
        cout << "salary : " << GetPay() << endl;
    } 
};

class PermanentWorker : public Employee
{
private :
    int cost;
public :
    int GetPay()
    {
        return Employee::GetPay() + 50;
    }
    void ShowInfo()
    {
        cout << "salary : " << GetPay() << endl;
    } 
};

// ShowInfo 는 완전히 동일한데 굳이 오버라이딩을 한 이유 ?
// ShowInfo() 함수는, 상속에 의해서 PermanentWorker 객체에도 존재하게 된다.
// 그러나 PermanentWorker ShowInfo() 에서 호출되는  GetPay() 함수는, (ShowInfo를 정의하지 않는다면)
// Employee 클래스에 정의된 GetPay 함수를 호출하게 된다.
// 따라서 PermanentWorker 클래스에 정의된 GetPay 함수 를 호출하게 하기 위해서
// 함수의 몸체 부분은 동일하더라도 , 별도로 ShowInfo 를 정의하는 것이다 .