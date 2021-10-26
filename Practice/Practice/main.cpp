// 프록시 패턴
// 아무리 어려운 문제도 대신할 객체(간접층)을 도입하면 해결할수 있다.

#define _CRT_SECURE_NO_WARNINGS

//문제
//	=>  참조계수로 동작하는 string 클래스를 생각해봅시다.
//	=> COW( Copy On Write ) 기법을 활용해서 String을 작성해봅시다

#include<iostream>
#include<cstdlib>
#include<string>
#include<stdio.h>

using namespace std;

class String
{
	int* ref;
	char* buff;
public:
	String(const char* s)
	{
		buff = new char[strlen(s) + 1];
		strcpy(buff, s);
		ref = new int(1);
	}
	String(const String& s) : buff(s.buff), ref(s.ref)
	{
		++(*ref);
	}
	//[] 연산자 : 객체를 배열처럼 보이게 한다.
	char& operator[](int index)
	{
		cout << "operator []" << endl;
		return buff[index];
	}
};
int main()
{
	// 첫번째 줄은 "대입"연산자
	// 두번째 줄은 "복사"생성자  
	String s1 = "hello";
	String s2 = s1; // 메모리를 잘 생각해보세요. 복사생성자를 보세요

	char c = s1[0]; // 읽는 코드. 자원은 계속 공유 되어야 한다.
	//cout << c << endl; // 'h'
	s1[0] = 'x'; // 쓰는 코드. 자원은 분리되어야 한다.
	// COW(Copy On Write)
	// cout << s1 << endl; // "xello"
	//cout << s2 << endl; // "hello"
}







