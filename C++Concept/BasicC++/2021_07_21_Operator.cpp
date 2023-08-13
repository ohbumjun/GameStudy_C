#include<iostream>

using namespace std;

/*
operator :
모든 연산자를 내가 원하는 대로
재정의를 해서 사용할 수 있는 기능이다.

구조체 혹은 클래스에서
oprator를 이용해서
원하는 연산자를 원하는 기능으로
만들어 줄 수 있다.

형태
반환타입 operator 재정의할연산자(인자)

*/
struct Point
{
	int x = 10;
	int y = 10;
	// operator는 함수이다.
	// operator는 사실 함수를 호출하는 것이다
	// pt1 + pt2를 한다면 pt1의 + 함수를 호출하는 것
	// + 함수의 경우, 인자로 pointer 타입 
	// 구조체를 받아오고 있다
	Point operator+(const Point& pt)
	{
		Point result;
		result.x = x + pt.x;
		result.y = y + pt.y;
		return result;
	}
	// 함수 overloading
	Point operator+(int value)
	{
		Point result;
		result.x = x + value;
		result.y = y + value;
		return result;
	}
	Point operator-(int value)
	{
		Point result;
		result.x = x - value;
		result.y = y - value;
		return result;
	}
	void operator +=(const Point& pt)
	{
		x += pt.x;
		y += pt.y;
	}
	// 전치 연산
	void operator++()
	{
		++x;
		++y;
	}
	// 후치 연산
	void operator++(int)
	{
		++x;
		++y;
	}
};

int main()
{
	// 왜 구조체끼리는 +,- 연산이 안되는 것일까
	// 구조체의 크기가 다 다르기 때문이다
	// 단, 별도의 + operator를 만들면 된다

	// pt1 + pt2 의 리턴값을 pt3에 저장
	Point pt1, pt2, pt3;
	pt3 = pt1 + pt2;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	pt3 = pt1 + 5;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	pt3 = pt1 - 5;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	pt3 += pt1;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	pt3++;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	++pt3;
	cout << "x : " << pt3.x << ", y : " << pt3.y << endl;

	return 0;
}