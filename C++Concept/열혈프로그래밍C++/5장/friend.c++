// friend 함수 예시
class Point;

class PointOP
{
    Point Add(const Point& p1, const Point& p1){}
};

class Point
{
private :
    int x;
    int y;
public :
    // 즉, PointOP의 Add 함수를 Point 객체의 friend로 선언하여
    // Point 의 private 멤벼변수에 접근할 수 있게 세팅한다.
    friend Point PointOP::Add(const Point&, const Point&);
};

Point PointOP::Add(const Point& p1, const Point& p2)
{
    return p1.x + p2.x;
}