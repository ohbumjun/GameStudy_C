#include<iostream>

// Remote
// Virtual
// Access 

// ���� Interface�� ������
// �ش� �ν��Ͻ� �� ���� Ref ����
// �ش� �ν��Ͻ� ����, �Ҹ꿡 ����

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