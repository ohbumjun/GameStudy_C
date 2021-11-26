#include<iostream>

// Remote
// Virtual
// Access 

// 같은 Interface를 따르고
// 해당 인스턴스 에 대한 Ref 유지
// 해당 인스턴스 생성, 소멸에 관여

struct Image
{
	virtual void draw() = 0;
};

struct Bitmap : public Image
{
	Bitmap(const std::string& fname) : filename(fname){}
	virtual void draw() override
	{
		std::cout << "draw bmp" << std::endl;
	}
	void load() { std::cout << "load bmp" << std::endl; }
private :
	std::string filename;
};

// Gives you the apperance of working with same object
// that you're used to working with
// even though object might not have been created

struct LazyBitmap : public Image
{
	LazyBitmap(const std::string& fName) : filename(fName){}
	virtual void draw()
	{
		if (!bmp)
			bmp = new Bitmap(filename);
		bmp->draw();
	}
private :
	std::string filename;
	Bitmap* bmp = nullptr;
};

int main()
{
	LazyBitmap bmp{ "pokemon.png" };
	bmp.draw();
	return 0;
}