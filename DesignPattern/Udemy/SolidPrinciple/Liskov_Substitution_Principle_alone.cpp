#include<iostream>

/*
liskov priciple 
- if S is a subtype of T, 
then objects of type T may be replaced with
objects of type S 

ex) Animal �̶�� class�� �ִ�
Animal�̶�� class�� ����ϴ� ��� ������ , 
�ش� sub class�� �ش��ϴ� ��ҿ� ��ü�Ͽ�
���� �� �־�� �Ѵ� 
ex) Dog <-- Animal 

Animal�� ����� ��� ������
Dog�� ����� �� �־�� �ϰ�
work fine �ؾ� �Ѵ�  
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
		// size,size --> �翬�ϴ�. ����,���� ���̰� �����ؾ� �ϹǷ�  
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
	// ���� process �Լ����ڷδ� Rentangle instance�� ���� �Ѵ�
	// �׷��� Rentalgle�� �ڽ�  Square�� �� �� �־�� �Ѵ�
	// ��, Ư�� class�� ���� ��ġ����
	// �ش� class�� ����� �ڽ� class �鵵 ��� �� �� �־�� �Ѵ�.
	// �̰��� �ٷ� Liskov_Substitution_Principle �� ���̴�   
	process(sq);  
	
	
	return 0;
}
