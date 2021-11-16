#include<iostream>

using namespace std;

struct Image
{
	virtual void draw() = 0;
};

struct Bitmap : public Image
{
	Bitmap(const string& filename)
	{
		cout << filename << endl;
		load();
	}
	void draw() { cout << "draw bitmap" << endl; }
	void load() { cout << "load" << endl; }
};

struct LazyBitmap : public Image
{
private :
	string filename;
	Bitmap *bmp = nullptr;
public :
	LazyBitmap(const string& filename) : filename(filename){}
	void draw()
	{
		if (!bmp)
			bmp = new Bitmap(filename);
		bmp->draw();
	}
};

int main()
{
	LazyBitmap bmp{ "pokemon.png" };
	// bmp.draw();

	return 0;
}