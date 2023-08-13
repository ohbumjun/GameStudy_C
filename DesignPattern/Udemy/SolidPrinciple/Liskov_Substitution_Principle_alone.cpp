#include<iostream>

/*
liskov priciple 
- if S is a subtype of T, 
then objects of type T may be replaced with
objects of type S 

ex) Animal 이라는 class가 있다
Animal이라는 class를 사용하는 모든 곳에서 , 
해당 sub class에 해당하는 요소와 대체하여
사용될 수 있어야 한다 
ex) Dog <-- Animal 

Animal을 사용한 모든 곳에서
Dog를 사용할 수 있어야 하고
work fine 해야 한다  
*/

using namespace std;
using namespace boost;

class Rectangle
{
	protected :
		int width, height;
	public :
		Rectangle(int width, int height) : width(width), height(height) {}	
		
		int getWidth() const {
			return width;
		}
		virtual void setWidth(int width){
			Rectangle::width = width;
		}
		int getHeight() const {
			return height;
		}
		virtual void setHeight(int height){
			Rectangle::height = height;
		}
		int area() const { return width * height;}
};

class Square : public Rectangle
{
	public :
		// size,size --> 당연하다. 가로,세로 길이가 동일해야 하므로  
		Square(int size) : Rectangle(size,size){}
		
		virtual void setWidth(int width) override {
			this->width = this->height = width;
		}
		
		virtual void setHeight(int height) override {
			this->width = this->height = height;
		} 	
};

void process(Rectangle& r)
{
	// get width of the shape, get it into variable
	int w = r.getWidth();
	// setHeight as 10 --> expect area to be equal as ( w * 10 )
	r.setHeight(10);
	cout << "expected area = " << (w*10) 
		<< ", got " << r.area()	<< endl;
	cout
}

int main()
{
	Rectangle r{3,4};
	process(r);
	
	Square sq{5};
	// 원래 process 함수인자로는 Rentangle instance가 들어가야 한다
	// 그런데 Rentalgle의 자식  Square도 들어갈 수 있어야 한다
	// 즉, 특정 class가 들어가는 위치에는
	// 해당 class를 상속한 자식 class 들도 모두 들어갈 수 있어야 한다.
	// 이것이 바로 Liskov_Substitution_Principle 인 것이다   
	process(sq);  
	
	
	return 0;
}
