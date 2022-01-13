#include<iostream>

using namespace std;

/*
operator :
��� �����ڸ� ���� ���ϴ� ���
�����Ǹ� �ؼ� ����� �� �ִ� ����̴�.

����ü Ȥ�� Ŭ��������
oprator�� �̿��ؼ�
���ϴ� �����ڸ� ���ϴ� �������
����� �� �� �ִ�.

����
��ȯŸ�� operator �������ҿ�����(����)

*/
struct Point
{
	int x = 10;
	int y = 10;
	// operator�� �Լ��̴�.
	// operator�� ��� �Լ��� ȣ���ϴ� ���̴�
	// pt1 + pt2�� �Ѵٸ� pt1�� + �Լ��� ȣ���ϴ� ��
	// + �Լ��� ���, ���ڷ� pointer Ÿ�� 
	// ����ü�� �޾ƿ��� �ִ�
	Point operator+(const Point& pt)
	{
		Point result;
		result.x = x + pt.x;
		result.y = y + pt.y;
		return result;
	}
	// �Լ� overloading
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
	// ��ġ ����
	void operator++()
	{
		++x;
		++y;
	}
	// ��ġ ����
	void operator++(int)
	{
		++x;
		++y;
	}
};

int main()
{
	// �� ����ü������ +,- ������ �ȵǴ� ���ϱ�
	// ����ü�� ũ�Ⱑ �� �ٸ��� �����̴�
	// ��, ������ + operator�� ����� �ȴ�

	// pt1 + pt2 �� ���ϰ��� pt3�� ����
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