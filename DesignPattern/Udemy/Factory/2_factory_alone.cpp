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
  // Point�� private factor�� ������ �� �ְ� �ϱ� ����  
  //  OCP �� ����ȴ� 
  // �Ϲ������� friend ��, OCP�� ����Ǵ� ������ �����ȴ�  
  // ����, �׳� publc���� �����, freind��
  /// ������� �ʴ� �͵� ����̴�  

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
  // ���� point�� ������ method����
  // PointFactory��� struct �� �ִ´� 
};

// concrete factory
// not abstract 

// ���� : Factory Pattern
// 1. in ordinary class ex) PointFactory
// which contains some methods
// 2. has to access members of other class
struct PointFactory
{
	// �� static�� �ʿ�� ����  
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
