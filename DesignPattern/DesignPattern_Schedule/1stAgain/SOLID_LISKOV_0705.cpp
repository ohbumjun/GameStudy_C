#include<iostream>

using namespace std;

struct Rectangle
{
protected :
	int width, height;
public :
	Rectangle(int width,int height):width(width),height(height){}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	virtual void setHeight(int height) {
		height = height;
	}
	virtual void setWidth(int width) {
		width = width;
	}
};

struct Square : public Rectangle
{
public :
	Square(int size) : Rectangle(size,size){}
	void setWidth(int width) {
		this->width = this->height = width;
	}
	void setHeight(int height) {
		this->height = this->height = height;
	}
};

void process(Rectangle& r)
{
	int w = r.getWidth();
	r.setHeight(10);
	cout << "expected area = " <<  (w*10) << endl;
}

int main()
{
	Rectangle r{ 4,5 };
	process(r);
	Square sq{ 5 };
	process(sq);
	return 0;
}