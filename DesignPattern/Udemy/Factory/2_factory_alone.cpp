#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

enum class PointType
{
  cartesian,
  polar
};

class Point
{
  /*Point(float a, float b, PointType type = PointType::cartesian)
  {
  if (type == PointType::cartesian)
  {
  x = a; b = y;
  }
  else
  {
  x = a*cos(b);
  y = a*sin(b);
  }
  }*/

  friend class PointFactory  
  // Point의 private factor에 접근할 수 있게 하기 위함  
  //  OCP 에 위배된다 
  // 일반적으로 friend 는, OCP에 위배되는 것으로 인지된다  
  // 따라서, 그냥 publc으로 만들고, freind를
  /// 사용하지 않는 것도 방법이다  

  Point(const float x, const float y)
    : x{x},
      y{y}
  {
  }

public:
  float x, y;


  friend std::ostream& operator<<(std::ostream& os, const Point& obj)
  {
    return os
      << "x: " << obj.x
      << " y: " << obj.y;
  }
  // 실제 point를 만들어내는 method들은
  // PointFactory라는 struct 에 넣는다 
};

// concrete factory
// not abstract 

// 정리 : Factory Pattern
// 1. in ordinary class ex) PointFactory
// which contains some methods
// 2. has to access members of other class
struct PointFactory
{
	// 꼭 static일 필요는 없다  
  static Point NewCartesian(float x, float y)
  {
    return{ x,y };
  }
  static Point NewPolar(float r, float theta)
  {
    return{ r*cos(theta), r*sin(theta) };
  }
}

int main_z()
{
  // will not work
  //Point p{ 1,2 };

  auto p = PointFactory::NewPolar(5, M_PI_4);
  std::cout << p << std::endl;
  
  Point p2{3,4};

  getchar();
  return 0;
}
