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
	// �̷��� �ϸ� friend�� �����ϰų� ( OCP �Ѽ� ) 
	// Point�� private �������� public���� ���� �ʿ䵵 ����
	// �� ���� ���  
	// "Inner Factory"
	// �� Inner Factory��� �Ҹ��� ���ϱ� ?
	// class PointFactory�� class Point �ȿ� �����ϱ� �����̴� 
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

