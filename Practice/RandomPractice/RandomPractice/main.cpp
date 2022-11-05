#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <cstring>

using namespace std;

struct Base
{
public:
	Base() { cout << "Base 생성자" << endl; }
	virtual ~Base() { cout << "Base 소멸자" << endl; }
public:
	virtual void func1() { cout << "Base의 func1" << endl; }
	virtual void func2() { cout << "Base의 func2" << endl; }
	void func3() { cout << "Base의 func3" << endl; }
public:
	int iValue;
	char cValue;
};

struct Child1 : Base
{
public:
	Child1() { cout << "Child1 생성자" << endl; }
	~Child1() { cout << "Child1 소멸자" << endl; }
public:
	virtual void func1() { cout << "Child1의 func1" << endl; }
	virtual void func2() { cout << "Child1의 func2" << endl; }
	void func3() { cout << "Child1의 func3" << endl; }
public:
	float fValue;
};

struct Child2 : Child1
{
public:
	Child2() { cout << "Child2 생성자" << endl; }
	~Child2() { cout << "Child2 소멸자" << endl; }
public:
	virtual void func1() { cout << "Child2의 func1" << endl; }
	virtual void func2() { cout << "Child2의 func2" << endl; }
	void func3() { cout << "Child2의 func3" << endl; }
public:
};

int main() {
	cout << sizeof(Base) << endl; // 16
	cout << sizeof(Child1) << endl; // 24
	cout << sizeof(Child2) << endl; // 24
}