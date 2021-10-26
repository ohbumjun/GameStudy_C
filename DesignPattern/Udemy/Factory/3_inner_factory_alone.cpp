#include <cmath>

enum class PointType
{
  cartesian,
  polar
};

class Point
{
public :
  // use a factory method
  Point(float x, float y) : x(x), y(y){}

// singleton
static PointFactory Factory; // make certain instance 
private :
  float x, y;
	// 이렇게 하면 friend를 선언하거나 ( OCP 훼손 ) 
	// Point의 private 변수들을 public으로 만들 필요도 없다
	// 더 좋은 방법  
	// "Inner Factory"
	// 왜 Inner Factory라고 불리는 것일까 ?
	// class PointFactory가 class Point 안에 존재하기 때문이다 
  	class PointFactory
	{
	  static Point NewCartesian(float x, float y)
	  {
	    return Point{ x,y };
	  }
	
	  static Point NewPolar(float r, float theta)
	  {
	    return Point{ r*cos(theta), r*sin(theta) };
	  }
	};
};

int main()
{
	auto p = Point::Factory.NewCatesian(3,4);
	// mauto p = Point::PointFactory::NewPolar(5,M_PI_4); 
	cout << p << endl;
	
	return 0;
 } 

