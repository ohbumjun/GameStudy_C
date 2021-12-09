#include<iostream>

using namespace std;

class Point
{
    int x,y;
public :
    Point(int px = 0, int py = 0) : x(px), y(py){}

    Point& operator ++ ()
    {
        x++;
        y++;
        return *this;
    }
    // ���� ����  
    const Point operator ++ (int) // ���ڿ� int�� �������ν�, ������������ ������ش�.
    {
        const Point obj(x,y);
        x++;
        y++;
        cout << "after ++ operator" << endl;
        return obj;
    };
    void ShowInfo() const 
    {
        cout << x << y << endl;
    }
    friend Point& operator --(Point& ref);
    friend const Point operator --(Point& ref, int);
};

Point& operator -- (Point& ref)
{
    --ref.x;
    --ref.y;
    return ref;
};

// ���� ����
const Point operator -- (Point& ref, int)
{
    const Point obj(ref.x,ref.y);
    --ref.x;
    --ref.y;
    return obj;
}

// Q. ���� �������� const�� �ٿ��� ����
int main()
{
    Point pos(1,2);
	const Point posAfter = pos++;
    posAfter.ShowInfo(); // 1,2
    pos.ShowInfo(); // 2.3
}

